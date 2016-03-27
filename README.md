# Project Carrot Anim Extractor

An utility program to unpack resources from an `Anims.j2a` file of a
Jazz Jackrabbit 2 installation. It is primarily targeted as a tool for obtaining a
set of assets for [Project Carrot](https://github.com/soulweaver91/project-carrot),
but can also be used for more general purposes.

Version compatibility:

* Pre-release versions (1.00g, 1.00h, 1.10o, etc.): **UNSUPPORTED**.
  Not tested, but expected to not work.
* Retail 1.20 (European) - **SUPPORTED**
* Retail 1.20 (American) - **PARTIALLY SUPPORTED**: Extraction will likely work,
  but filename assignments will likely be wrong at this time. Not tested.
* Patched retail 1.23 (European) - **UNKNOWN**: Not tested, assumed to work.
* Patched retail 1.23 (American) - **UNKNOWN**: Not tested. If works, likely
  to same degree as American 1.20.
* Retail 1.23x (Holiday Hare '98): **UNKNOWN**
* Retail 1.24 (The Secret Files) - **SUPPORTED**
* Retail 1.24x (Christmas Chronicles) - **UNKNOWN**: Not tested, assumed to work.

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
Configure the Qt settings with the MSVS Qt 5 plugin, then set up the user macro
`SFML_DIR` in the property sheet to point to the location of your SFML installation.

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