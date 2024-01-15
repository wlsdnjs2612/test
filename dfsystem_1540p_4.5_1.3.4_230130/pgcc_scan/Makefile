CC = g++
VER = 2018_03_09
TARGET = pgcc-df_$(VER)
TARGET_LINK = pgcc-df
SOURCE = pgcc.cpp
HEADER = pgcc.h
#CXXFLAGS = -I.. -pipe -O2 -W -Wall
CXXFLAGS = -I.. -D_VERSION_=\"$(VER)\" -O2 -W -Wall 
LDFLAGS = -L.. -ldfsystem
REAL_DIR = df/
TARGET_BASEDIR = /opt/dfsystem/bin/
TARGET_REALDIR = $(TARGET_BASEDIR)$(REAL_DIR)

$(TARGET): $(SOURCE) $(HEADER)
	$(CC) -o $(TARGET) $(SOURCE) $(CXXFLAGS) $(LDFLAGS)

clean:
	-rm -f $(TARGET)

copy:
	@make install

install:
	-mkdir $(TARGET_BASEDIR) $(TARGET_REALDIR)
	cp -rf $(TARGET) $(TARGET_REALDIR)
	chmod 755 $(TARGET_REALDIR)$(TARGET)
	-cd $(TARGET_BASEDIR) && rm -rf $(TARGET_LINK)
	-cd $(TARGET_BASEDIR) && ln -s $(REAL_DIR)$(TARGET) $(TARGET_LINK)
