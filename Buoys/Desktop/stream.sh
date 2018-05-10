#!/bin/sh

arecord -D plughw:1,0 -f dat | sshpass -p 'ramboat' ssh -C whale-srv@169.254.21.169 aplay -f dat &
