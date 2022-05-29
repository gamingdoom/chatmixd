# chatmixd
A daemon for linux to check if the chatmix dial on your arctis headset is in the middle.

## Building
Git clone the repo and make:
```
git clone https://github.com/gamingdoom/chatmixd.git
cd chatmixd
make
```
## Installation
The binary will be build/chatmixd after you run make. You can install it and the systemd service with these commands:
```
cp build/chatmixd /usr/local/bin/chatmixd
```
Edit chatmixd.service to your liking, then:
```
cp chatmixd.service ~/.config/systemd/user/
```
Finally enable and start the service:
```
systemctl enable --user --now chatmixd
```
