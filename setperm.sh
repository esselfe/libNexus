#!/bin/bash

find . -type d -exec chmod 755 '{}' \;
find . -name *.c -exec chmod 644 '{}' \;
find . -name *.h -exec chmod 644 '{}' \;
find . -name Makefile -exec chmod 644 '{}' \;
find . -name *mesh -exec chmod 644 '{}' \;
find . -name *.png -exec chmod 644 '{}' \;
chmod 644 {ChangeLog,Readme,idea,make.output,*cfg,pseudo}
