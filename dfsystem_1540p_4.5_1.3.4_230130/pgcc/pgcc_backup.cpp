// pgxx.cpp : Defines the entry point for the console application.
//
#include <tgcompiler.h>
#include <pgcompiler.h>
//#include <iostream.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
/////////////////////////////////////////////////////////////////////////////
// The one and only application object

bool		pc_line ;
char		*src_org ;
int		sysModel ;
String		strSrcFile ;
timesetMap	tgGroup ;
TGCompiler	*timeCompiler=NULL;

String deviceDefault( char *source ) {
	char  *start , *end ;
	String defSrc="" ;

	if( ( source=strstr( source , "\nvoid device_default(" )) ) {
		if( (start = strchr( source , '{' )) &&
			(end = strchr( source , '}' )) ) {
			*end=0;
			defSrc =  start ;
			*end = '{' ;
		}
	}
	return defSrc ;
}

void eatComment( char *source ) {
	char	*s;
	int	start , end ;

	s=source;
	while( s && (s=strstr(s, "/*")) ) {
		start = s-source+2 ;
		
		if( s && (s=strstr(s+2, "*/")) ) {
			end = s-source-2;
			memset( source+start , ' ' , end-start ) ;
		}
	}
	s=source;
	while( s && (s=strstr(s,"//")) ) {
		start =  s-source ;

		if( s && (s=strchr(s+2, '\n')) ) {
			end = s-source;
			memset( source+start , ' ' , end-start ) ;
		}
	} 
}

void getWord( char *buf , char *buf1 ) {
	char *s ,*e;
	char tmp[132]={0,};
		
	if( (s=strchr( buf , '(') ) && (e=strchr( buf , ')')) ) {
		strncpy( tmp , s+1 , e-s-1 );
		sscanf( tmp , " %s " , buf1 );
	}	
}

String getFile( const char *strFile ) {
	char *s ;
	String strString ;
	int   size ;

	FILE*fp = fopen( strFile , "r" );
	
	if( fp ) {
		fseek( fp , 0L , SEEK_END );
		size = ftell( fp ) ;
		fseek( fp , 0L , SEEK_SET) ;
		
		s = new char [ size+1 ] ;
		fread( s , size , 1 , fp ) ;
		s[size]=0;
		strString = s ;
		fclose(fp);
		delete s ;
	}
	return strString ;
}

int countLine( int start , int end , const char *buf ) {
	int line =0 ;

	for(int i= start ;i< end ; i++ ) {
		if( buf[i] == '\n' ) line++ ;
	}
	return line ;
}

char * Clock( int pin , char *name ) {
	static char assign[30]= {0,} ;

	if( name[0] == 'x' || name[0] == 'X') return NULL ;

	assign[0]='\0';

	if( pin < 28 ) {
		sprintf( assign + strlen(assign) , "assign_pin\t%s\tC%d \n" , name , pin );
	}
	else {
		     if( pin == 27 ) sprintf( assign + strlen(assign) , "assign_pin\t%s\tamux "     , name );
		else if( pin == 28 ) sprintf( assign + strlen(assign) , "assign_pin\t%s\tdmux "     , name );
		else if( pin == 29 ) sprintf( assign + strlen(assign) , "assign_pin\t%s\tstrobe "   , name );
		else if( pin == 30 ) sprintf( assign + strlen(assign) , "assign_pin\t%s\ttri "      , name );
	}
	return assign ;
}

bool TimesetCompile( const char *buffer ) {
	char group[32]="yskim" , name[32] , width[10] , name2[32] , width2[10];

	char *group_end, *sub_end , *s ;
	char *buf = (char *)buffer ;

	while( ( buf=strstr( buf , "\nTIMESET(" )) ) {
		if( (group_end = strchr( buf , '}' )) ) *group_end ='\0';
		else break ;

		getWord( buf , group ); 
		if( ! tgGroup.Lookup( group , timeCompiler ) ) {
			timeCompiler = new TGCompiler ;
			timeCompiler->setModel( sysModel);
			tgGroup[ group ] = timeCompiler ;
		}
#ifdef FOX
		char *ts=buf ;
		if( (buf=strstr( buf , "CLOCK" )) ) {
			if( (buf=strchr( buf , '(' )) ) {
				if( (s=strchr( buf , ')' )) ) {
					*s='\0' ;

					int	    use=0;
					String str_assign ;
					do {
						if( (c = strchr( buf , ',' )) ) *c= ' ' ;
						if( sscanf( buf+1 , "%s" , pin ) ) {
							str_assign += Clock( use++ , pin ) ;
						}
						buf = c ;
					}
					while( c ) ;
					buf = s+1 ;

					if( timeCompiler->ts()->chlist.GetCount() == 0 ) {
						int line = countLine( 0 , s-buffer , buffer ) ;
						timeCompiler->compile((char *)(const char *)str_assign ,line , strSrcFile );
						if( timeCompiler->error() ) {
							printf( timeCompiler->getError() );
							return false;
						}
					}
				}
			}
		}
		buf=ts ;
#endif		
		
		while( (buf=strstr( buf , "SET_TS" )) ) {
			if( (s = strchr( buf , '(' )) ) {
				sscanf( s+1 , "%[^,],%[^,]" , name2 ,width2 ) ;
				 
				sscanf( name2 , "%s" , name);
				sscanf( width2 , "%s" , width);

				s=strchr( s , ',' );
				if( (s=strchr( s , '\n' )) ) {
					s += 1 ;
					if( (sub_end= strchr( s , ')')) ) {
						*sub_end = '\0';
						buf = sub_end+1 ;
					}
//					if( (sub_end= strrchr( s , '\n')) )  *sub_end = '\0';

					while( strchr(s ,'\n') ) *(strchr(s ,'\n'))=' ' ;

					String str_tsn ;
				        str_tsn	= "ts=" ;
					str_tsn += name ;
					str_tsn += " {  width=";
					str_tsn +=  width;
					str_tsn += " { ";
					str_tsn += s ;
					str_tsn += " } ";

					int line = countLine( 0 , s-buffer , src_org ) ;

					timeCompiler->setLogo( strSrcFile );
					timeCompiler->splitSyntex((char*)str_tsn , line+1 );
					if( timeCompiler->error() ) {
						printf( timeCompiler->getError() );
						return false;
					}
				}
			}
		}

		buf = group_end + 1;
	}
	if( timeCompiler == NULL ) {
		char *p ;
		buf = (char *)buffer ;

		timeCompiler = new TGCompiler ;
		timeCompiler->setModel( sysModel);
                tgGroup[ group ]= timeCompiler ;

		while( ( buf=strstr( buf , "SET_TS(" )) ) {
			if( (group_end = strchr( buf , '\n' )) ) *group_end ='\0';
			else break ;

			p = strtok( buf+3 , "\" ,);");
			while( p ) {
				TTGDATA *tg = new TTGDATA;
				strcpy( tg->name , p ) ;
				timeCompiler->ts()->list.AddTail( tg ) ;
				p = strtok( NULL , "\" ,);");
			}
			buf = group_end+1 ;
		}
	}

	return true ;
}

bool PatternCompile( const char *buffer ) {
	patternMap	patternGroup ;
	PGCompiler	*patternCompiler=NULL;
	String 		str , strPC;
	bool		nosub=false , nogroup=false;
	char		strSubName[102400]="";

	char group[32] ,name[64] , ts_group[20]="yskim" ;
	char *group_end, *sub_end , *s ;
	char *buf = (char *)buffer ;

	String def_src ;
	def_src	= deviceDefault( buf ) ;

	while( ( buf=strstr( buf , "\nPATTERN(" )) ) {
		if( patternCompiler ) {
			patternCompiler->init();
			delete patternCompiler ;
		}
		group_end=0;

		s=buf ;

		if( (s=strstr( s , "SET_PATTERN") ) == 0 ) s=buf ; 
		if( (group_end = strchr( s , '}' )) ) *group_end ='\0';
		else break ;
		
		//sscanf( buf , "\nPATTERN(%s)", group ) ;
		getWord( buf , group );
		//printf("-%s-",group);  

		strcat( strSubName , "_group_start" );
		strcat( strSubName , group );
		strcat( strSubName , " " );

		if( ! patternGroup.Lookup( group , patternCompiler ) ) {
			patternCompiler = new PGCompiler ;
			patternCompiler->setModel( sysModel );
			patternGroup[ group ]= patternCompiler ;
		}

		if( (s=strstr( buf , "LOAD" )) ) {
			if( (s=strstr( s , "TIMESET" )) ) {
				if( (s=strchr( s , ',' )) ) {
					sscanf( s+1 , " %s " , ts_group) ;
				}
			}
		}
		if( tgGroup.Lookup( ts_group , timeCompiler ) )  {
			int timeset = timeCompiler->ts()->timeset ;

			for( POSITION pos = timeCompiler->ts()->list.GetHeadPosition();pos;timeset++) {
				TTGDATA *tg= timeCompiler->ts()->list.GetNext(pos) ;
				patternCompiler->addTsGroup( tg->name , timeset ) ;
			} 
		}			
		else {
			int line = countLine( 0 , s-buffer , src_org ) ;
			printf("%s:%d: '%s' undeclared (first use this timeset)\n" , (char *)strSrcFile ,line+1 , ts_group );
			return false;
		}

		bool device_default=false ;
		if( strstr( buf , "device_default") ) device_default = true ;

		char *org = buf ;
		char *org_buffer = (char*)buffer ;
		for( int i=0;i<1+device_default;i++) {
			if( device_default ) {
				if( i ) {
					buf = org ;
					buffer = org_buffer ;
				}
				else {
					buf =  (char *)def_src;
					buffer = buf;
				}
			}
			while( (buf=strstr( buf , "SET_SUB" )) ) {
				sub_end=0;
				if( (s = strchr( buf , '(' )) ) {
					sscanf( s+1 , " %s" , name  ) ;
	
					if( name[strlen(name)-1]==',' ) name[strlen(name)-1]=0;
					strcat( strSubName , name );
					strcat( strSubName, " " );

					if( (s=strchr( s , ',' )) ) {
						s += 1 ;
						
						if( (sub_end= strchr( s , ')')) ) {
							*sub_end = '\0';
							buf = sub_end+1 ;
						}
						if( strchr( s , ',' ))  *(strchr( s , ',' ))= ' ' ;

						String str_sub ;
					        str_sub	= "sub=" ;
						str_sub += name ;
						str_sub += " {  ";
						str_sub += s ;
						str_sub += " } ";

						int line = countLine( 0 , s-buffer , src_org ) ;
					
						patternCompiler->setLogo( strSrcFile );
						patternCompiler->splitSyntex((char *)(const char *)str_sub , line+1);
						if( patternCompiler->error() ) {
							printf( patternCompiler->getError() );
							return false;
						}
						else if( patternCompiler->pc() > 510 ) {
							printf("%s:%d:too large pattern line '%s'\n" , (char *)strSrcFile ,
								line+1 , name );
							return false;
						}
					}
				}
				if( sub_end ) *sub_end = ')' ;
			}
		}
		if( pc_line && ! patternCompiler->error() ) {
			str.sprintf("'%s'=%d " , group , patternCompiler->pc() );
			strPC += str ;
		}
		if( group_end ) *group_end='}';
		buf = group_end + 1;
		strcat( strSubName , "_group_end " );
	}

#ifndef FOX	
	bool	pre_process ;
	char    *x , *next=0;
	bool	find ;
        buf = (char *)buffer ;
	char    groupname[100]="";

        while( ( buf=strstr( buf+4 , "LOAD( PATTERN" ))  )  {
		sscanf( buf , "LOAD( PATTERN , %s" , name ) ;

		if( (x=strchr(name , ')')) ) *x=0;

		strcpy( groupname , "_group_start" );
		strcat( groupname , name ) ;
	       	strcat( groupname , " ");
		
		if(! (s=strstr( strSubName , groupname )) ) {
			printf("%s:%d:no group '%s'\n" , (char *)strSrcFile , countLine( 0 , buf-buffer , src_org )+1 ,name );
			buf += 20 ;
			nogroup=true ;
			continue ;
		}
		group_end=strstr( s , "_group_end" );	
		if( group_end ) *group_end=0;
				
		if( (next=strstr( buf+20 , "LOAD( PATTERN" )) ) *next=0;

		if( ( x=strstr( buf , "MPAT" ))  ) {
			pre_process=false;
			buf =x ;

			find = false;
			while( x--) {
				if( *x == '\n' || *x == ' ' ) break;
				else if( *x == '#' ) {
					pre_process = true ;
					break ;
				}
			}
			sscanf(x, " %[^(]" , name ) ;

			if( strcmp(name , "START_MPAT" ) && strcmp(name , "MEAS_MPAT")  ) {
				buf += 4 ;

				if( next ) *next='L';
				continue ;
			}

			if(! pre_process && sscanf( buf , "%[^\"]\"%[^\"]\"" ,group, name) == 2 ) {
//				       	(sscanf( buf , "%[^(]( %s" ,group, name  ) == 2 ||
				find = false ;
				while( (s=strstr(s,name)) ) {
					if((x=strchr( s ,' ' )) ) {
						*x=0;
						if( strcmp( s , name ) == 0 ) {
							if( isspace(*(s-1)) ) {
								find = true ;
								*x=' ';
								break ;
							}
						}
						*x=' ';
					}
					s += 1 ;
				}
				if(! find ) {
					nosub=true ;
					printf("%s:%d:no sub '%s'\n" , (char *)strSrcFile , 
							countLine( 0 , buf-buffer , src_org )+1 ,name );
				}	
			}
		}
		buf += 4 ;
		if( next ) *next='L';

		if( group_end ) *group_end = '_' ;
	}
/*        buf = (char *)buffer ;
        while( ( buf=strstr( buf , "AGING" )) ) {
		if( sscanf( buf , "%[^(]( %s" ,group, name  ) == 2 ) {
			bool find = false ;
			s = strSubName ;
			while( (s=strstr(s,name)) ) {
				if((x=strchr( s ,' ' )) ) {
					*x=0;
					if( strcmp( s , name ) == 0 ) {
						find = true ;
						*x=' ';
						break ;
					}
					*x=' ';
				}
				s += 1 ;
			}
			if( !find ) {
				nosub=true ;
				printf("%s:%d:no sub '%s'\n" , (char *)strSrcFile, countLine( 0 , buf-buffer , src_org )+1 ,name );
			}
		}
		buf += 4 ;
	}
*/	if( nogroup || nosub ) return false ;

	if(! strPC.isEmpty() ) printf("%s\n" , (char*)strPC );
#endif	
	return true ;
}

int main(int argc, char* argv[]) {
	String	strOutFile ;
	String	strFile ;
	char *	file_buffer;
	String	strPattern;
	String	strTimeset ;
	FILE		*fp ;
	bool	source=false ;

	for( int i=1;i< argc ; i++ ) 	{
		if( strstr(argv[i] , "-c" ) )strSrcFile = argv[++i] ;
		else if( strstr(argv[i] , "-o" ) )strOutFile = argv[++i] ;
		else if( strstr(argv[i] , "-s" ) ) source = true ;
		else if( strstr(argv[i] , "-n" ) ) pc_line = true ;
		else if( strstr(argv[i] , "-t33" ) ) sysModel=33 ;
		else if( strstr(argv[i] , "-t10" ) ) sysModel=10 ;;
	}

	if(! strSrcFile.isEmpty() ) {
		fp = fopen( (char *)strSrcFile , "r" ) ;
		if( fp ) {
			fseek( fp , 0L , SEEK_END ) ;
			int size = ftell(fp);
			fseek( fp , 0L , SEEK_SET ) ;
			
			file_buffer = new char [ size+1 ] ;
			fread( file_buffer , 1 , size , fp );
			file_buffer[size]=0;
			strFile = file_buffer ;
			src_org =(char *) strFile;

			eatComment( file_buffer );

			strPattern =  file_buffer ;
			strTimeset =  file_buffer ;
			delete file_buffer ;
			fclose(fp);

		}
		else {
			printf("File could not be opened\n");
			return 1 ;
		}
	}
	else {
		printf( "Command line error.\n" );
		return 1 ;
	}

	if( TimesetCompile( strTimeset ) && PatternCompile( strPattern ) )	{
		if(! strOutFile.isEmpty() ) {
			fp = fopen((char*) strOutFile , "w" ) ;
			if( fp ) {
				String str ;
				str = (char *)strFile ;

				str.replace( "\\" , "\\\\" );
				str.replace( "\"" , "\\\"" );
				str.replace( "\n" , "\\n\"\n\"" );
				str.replace( "%" , "%%" );
				str += "\";\n}\n" ;

				if( source ) {
					fprintf( fp , "extern \"C\" char *getSource() {\n\treturn \"" );
					fprintf( fp , (char *)str );
				}
				fprintf( fp , "extern \"C\" int getTime() {\n\treturn " );
				str.sprintf("%d ;\n}\n" ,(int) time(0) );
				fprintf( fp , (char *) str );
				fclose(fp);
			}
		}
	}
	else return 1;
	return 0;
}
