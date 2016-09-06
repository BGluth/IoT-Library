##### Current status:

(5/9/2016)
- Please note that the library is currently not in a stable state. The feature set is minimal for what is needed in another project (LINK HERE!!!) and this library has only been minimally tested.

## What?

A simple library whose purpose is to ease the work required to create IoT devices by putting as much common IoT functionality as possible into a single library.

Aims of the library:

- Portability
	- Written in C
	- Avoids using any feature that is specific to a certain compiler
	- Allows registering user defined functions to perform most of the functions of the library
		- ie. The user defines a function to upload data to a server backend
- Speed 
    -  Probably not considered fast yet
	- Only uses malloc when necessary

## Why?

I was working on a IoT project that had a few feature requirements that would be very common among IoT devices. These included:
- not polling a sensor when the current environment temperature was outside of the sensor's operating temperature range
- not polling a sensor if it's been polled too recently
- storing unsent sensor data locally if no connection can be made to the Internet
- uploading sensor data to a server backend

I was very surprised that I could not find any another generic IoT library. IoT development seems like such a good candidate for a library to remove all of the repetitive coding tasks that need to be done for every device. For example, probably most IoT devices do the following:

For each sensor:
- poll periodically
- timestamp sensor poll data
- do both or either:
	- upload the sensor readings periodically to a server backend
	- store data locally to non-volatile memory

In addition to this core functionality, other functionality could be added into the library that certain IoT devices might want to use, such as:
- only polling a sensor if the outside temperature is in its operating range
- have polling rates specific for each sensor
- setting a required warm up time for each sensor (in progress)
- library code that can be compiled with some extra debugging information

Since I couldn't find a library and I knew that I was likely going to be working on multiple IoT projects, I thought the most logical choice was to try to group the reusable code into a library.

## Documentation

Documentation is written with Doxygen. I don't really want to be constantly updating tracked generated html documentation, and would prefer to use something like [ReadTheDocs](https://readthedocs.org/). However, in the meantime, although this is inconvenient, the documentation needs to be generated manually. Fortunately, it's not as painful as it sounds, and all that needs to be done is this:

1. Install Doxygen
	- Make sure it adds itself to the PATH!
2. In terminal/command prompt, navigate to the /doc directory and run "doxygen".

Now to read the documentation, just open "index.html" inside of the newly generated /docs/html directory inside of /docs.

## Development

If you're interested in contributing to this library, issues and pull requests are totally welcome!

##### Testing

**Note:** The files below are written with the ".exe" extension. If you are running Linux/Mac, the file names will be missing the extension.

The C++ testing framework Catch was used for testing. The test runner was built with a **MinGW-64** compiler but it should work with others as well. Feel free to create an issue if it doesn't work with another compiler.

For running the tests, you'll need to run a build script which is not a GNU Make script, which makes running the tests a bit more work. The build script is written in Haskell using a library called [Shake](http://shakebuild.com/), which handles very similarly to Make except without a lot of the headaches (at least for me) and allows for much more expressive build systems, since you're really writing the build system itself in a full-fledged programming language.

The build script (build.hs) produces an executable (_shake/build.exe) which builds the testrunner (runTests.exe) when executed. However, to initially start running tests, build.exe is going to be missing, so this will need to be built first. You're going to need to install the Haskell stack (see the [Using Shake](http://shakebuild.com/#using-shake) section on the Shake page). Once installed, running "stack exec build.(bat/sh)" in the /tests directory should build build.hs, output build.exe, and automatically executes build.exe which will output runTests.exe. To be put more simply, you can build the tests by simply calling "stack exec build.bat" or "stack exec build.sh" in the /tests directory and then run "runTests.exe" in "tests".

Notes:
- There was an issue with the stable version of Stack that I used when writing the build script which caused "stack exec build.bat" to fail when run in command prompt. A fix for this was out there at the time but had not yet made it into the stable release of Stack.
- The tutorial for Shake shows running the .bat/.sh just by running the file itself without "stack exec". However, this will not work. If you install the Haskell compiler (ghc) through installing Stack (which is what Shake says you should do), Stack purposely does not add ghc to PATH in order to insulate the ghc toolchain from the rest of the system and force you to go through Stack. Since build.(bat/sh) calls ghc directly, it will not find ghc if called directly. Instead, calling it with "stack exec build.(bat/sh)" will allow it to find ghc.
- Testing is fairly inflexible at the moment due to hardware setup settings being baked right in during compilation (see user_settings.h). Hopefully in the future this can be worked around...

## Other Notes

The development of this project was funded by the City of Edmonton. (ADD MORE INFORMATION HERE)

This is really the first project that I've written in pure C and also the first project that I've had to enter the mindset of working in embedded land. Throughout the development of this library, I've been keeping speed and small binary sizes at the back of my mind (along with making it totally cross platform). However, since I'm still pretty new to embedded and C, I know it's quite likely that there are things that I could be doing better. If you have any questions/suggestions, I encourage you to voice them! Issues/pull requests are also totally welcome and very appreciated!
