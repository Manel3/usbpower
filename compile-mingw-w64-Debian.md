## Cross-compile in mingw-w64 on Debian

### Needed packages

```
# To develop in general
apt-get -y install g++ gcc make libc6-dev geany geany-plugin-debugger zip unzip

# To crosscompile to win libusb (and usbpower)
apt-get -y install mingw-w64 git autoconf automake libtool libudev-dev
```

### Get it
```
git init
git clone git://github.com/manel3/usbpower.git
```
### Submit changes
TODO & TOTEST
```
git add <filename>
git add *
git commit -m "What changed message"
git push origin master
```
