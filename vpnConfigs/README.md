Config for VPN which **only** tunnels IPs in Range 10.13.13.0/24.
Above config files include their configured IP in their name.

## Install (Ubuntu/Mint):
- `sudo apt install wireguard`
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

https://www.sigmdel.ca/michel/ha/wireguard/wireguard_02_en.html
https://www.raspberrypi.org/documentation/configuration/wireless/access-point-bridged.md

https://github.com/cSharpler2946/HSP-RPOSC-Redpitaya-Oszilloskop.git
