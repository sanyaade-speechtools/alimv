# Download #
Download alimv-mix-0.0.1.tar.gz from [here](http://code.google.com/p/alimv/downloads/list).
# Unpack and run with AliEn plugin arguments #
```
tar xfz alimv-mix-0.0.1.tar.gz
cd alimv-mix-0.0.1

// run on proof full mode (it will run on proof cluster)
root runALICE.C

// run on proof in test mode
root 'runALICE.C("proof","full")'

// run on grid with full
root 'runALICE.C("grid","full")'

// and so on (this macro is using AliEn plugin)
root 'runALICE.C("<source>","<mode>")'

```