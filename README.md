# ScriptCut
Shortcut, but for scripts

## Build and Run
### Linux
```
// Build from root
cd build
cmake ..
make

// Run from runtime directory
cd ../runtime
./ScriptCut
```

### Windows
```
// Build from root
cd build
cmake ..

// Run from runtime directory
// Open *.sln file generated from previous step in VS
// ctrl+f5
```

### Docker
Follow this video to get XLauncher setup:
[YouTube](https://www.youtube.com/watch?v=BDilFZ9C9mw&t=0s)

```
// Run from root to build
docker build -t scriptcut .

// Run from root to run
docker run -d scriptcut
```