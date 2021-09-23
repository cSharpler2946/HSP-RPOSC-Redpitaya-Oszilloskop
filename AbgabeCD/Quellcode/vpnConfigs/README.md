Config for VPN which **only** tunnels IPs in Range 10.13.13.0/24.
Above config files include their configured IP in their name.

## Install (Ubuntu/Mint):
- `sudo apt install wireguard`
- `sudo pacman -s wireguard-tools` for pacman
- Save .conf file to /etc/wireguard
- To start VPN: `wg-quick up <nameofconf>`
- To stop VPN: `wg-quick down <nameofconf>`
- If error `resolvconf: command not found` run `sudo ln -s /usr/bin/resolvectl /usr/local/bin/resolvconf`
- `sudo wg show all` shows the current connection

## Install (Windows):
- Install: https://download.wireguard.com/windows-client/wireguard-installer.exe
- Run and import config file

## Server Install (only for me as note!):
- Install wireguard, kernel-headers on host system
- Run docker-compose

for automatic startup use the wireguard build in service by following this link
https://www.ivpn.net/knowledgebase/linux/linux-autostart-wireguard-in-systemd/

to install wireguard on raspberrypi use this link
https://www.sigmdel.ca/michel/ha/wireguard/wireguard_02_en.html

To use raspberry as man in the middle, we used the raspberry vnc server and connected with realVncViewer (link below). Redpitaya connected via LAN to the Raspberry allows Web access via (rp-f05007.local) and ssh via root@rp-f05007.local. Of course you need to be connected to the VPN-Server with wireguard to access the raspberry and the RP.
https://www.realvnc.com/de/connect/download/viewer/
User: pi
PW: raspberry
