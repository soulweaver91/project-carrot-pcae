# Project Carrot Anim Extractor

An utility program to unpack resources from an `Anims.j2a` file of a
Jazz Jackrabbit 2 installation. It is primarily targeted as a tool for obtaining a
set of assets for [Project Carrot](https://github.com/soulweaver91/project-carrot),
but can also be used for more general purposes.

You can download prebuilt Windows binaries over at the 
[Project Carrot homepage](https://carrot.soulweaver.fi/).

Version compatibility:

* Pre-release versions (1.00g, 1.00h, 1.10o, etc.): **UNSUPPORTED**.
  1.00g tested; animation mappings could be extracted, but their mappings are
  somewhat different, while sound samples are in a currently unsupported format,
  effectively blocking the process for now.
* Retail 1.20 (European) - **SUPPORTED**
* Retail 1.20 (American) - **SUPPORTED?**: Based on HH98, the intro differences
  (Gathering of Developers logo instead of Project Two Interactive) do not
  affect the contents of the animation library in the end. This has not been
  tested, though; help from someone with access to an U.S. copy of the game would
  be appreciated.
* Patched retail 1.23 (European) - **SUPPORTED**
* Patched retail 1.23 (American) - **SUPPORTED?**: Same as 1.20.
* Retail 1.23x (Holiday Hare '98): **SUPPORTED**
* Retail 1.24 (The Secret Files) - **SUPPORTED**
* Retail 1.24x (Christmas Chronicles) - **SUPPORTED**

Compatibility may be improved at a later date.

## Usage

```
pcae path\to\Anims.j2a
```

PCAE will extract the animations and sound effects from the Anims.j2a file to a new
folder `Assets` into several subdirectories based on a predefined mapping.
Files are overwritten if they already exist. Subfiles with no matching
mapping are saved to the asset folder root.

## Development

Compiling requires [SFML 2.x](http://www.sfml-dev.org/download.php),
[Qt 5.x](http://www.qt.io/download/) and Microsoft Visual Studio 2015.
Configure the Qt settings with the MSVS Qt 5 plugin, then set up the macro
`SFML_DIR` in the user property sheets (`Microsoft.Cpp.Win32.user` and 
`Microsoft.Cpp.x64.user`) to point to the location of your SFML installation.

MSVS is not strictly required as long as you know how to set up a build
system manually, though.

## License
This software is licensed under the [MIT License](https://opensource.org/licenses/MIT).

Jazz Jackrabbit 2 and all related assets are intellectual property of Epic Games.
By using this software, you are fully taking all responsibility from the extraction
process.

## Thanks
This software could not exist without the groundwork laid by the users of the Jazz2Online
forums in the past years. Many of the details of the `Anims.j2a` file were obtained from
[the format research thread](http://www.jazz2online.com/jcf/showthread.php?t=15059).