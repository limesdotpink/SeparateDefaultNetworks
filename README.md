# SeparateDefaultNetworks

A simple WUPS plugin allowing you to set separate default networks for Wii U and vWii.
<table>
  <tr>
    <td>
      <img width="854" height="480" alt="Wii U system settings showing one network connection set as default for Wii U, and another for Wii" src="https://github.com/user-attachments/assets/a1b07760-e533-446b-b5ee-f4b72a513460" />
    </td>
    <td>
      <img width="854" height="480" alt="Plugin settings interface." src="https://github.com/user-attachments/assets/cc2f3a76-e21e-41d8-a3d7-d6f77d0cfb93" />
    </td>
  </tr>
</table>
    

## Installation

(`[ENVIRONMENT]` is a placeholder for the actual environment name.)

1. Copy the file `SeparateDefaultNetworks.wps` into `sd:/wiiu/environments/[ENVIRONMENT]/plugins`.
2. Requires the [WiiUPluginLoaderBackend](https://github.com/wiiu-env/WiiUPluginLoaderBackend) in `sd:/wiiu/environments/[ENVIRONMENT]/modules`.

Start the environment (e.g Aroma) and the backend should load the plugin.

## Building

For building you need:

- [wups](https://github.com/Maschell/WiiUPluginSystem)
- [wut](https://github.com/devkitpro/wut)

Install them (in this order) according to their README's. Don't forget the dependencies of the libs itself.

Then you should be able to compile via `make` (with no logging) or `make DEBUG=1` (with logging).

## Buildflags

### Logging

Building via `make` only logs errors (via OSReport). To enable logging via the [LoggingModule](https://github.com/wiiu-env/LoggingModule) set `DEBUG` to `1` or `VERBOSE`.

`make` Logs errors only (via OSReport).  
`make DEBUG=1` Enables information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).  
`make DEBUG=VERBOSE` Enables verbose information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).

If the [LoggingModule](https://github.com/wiiu-env/LoggingModule) is not present, it'll fallback to UDP (Port 4405) and [CafeOS](https://github.com/wiiu-env/USBSerialLoggingModule) logging.

## Building using the Dockerfile

It's possible to use a docker image for building. This way you don't need anything installed on your host system.

```
# Build docker image (only needed once)
docker build . -t setdefaultnetwork-builder

# make 
docker run -it --rm -v ${PWD}:/project setdefaultnetwork-builder make DEBUG=1

# make clean
docker run -it --rm -v ${PWD}:/project setdefaultnetwork-builder make clean
```

## Format the code via docker

`docker run --rm -v ${PWD}:/src ghcr.io/wiiu-env/clang-format:13.0.0-2 -r ./src -i`
