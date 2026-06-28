# Short mDNS hostname so avahi-daemon publishes the board as
# power-window.local. If you have multiple boards on
# the same LAN you'll want to override this per-board, since avahi
# will rename collisions to `power-window-2.local` etc.
hostname:power-window = "power-window"