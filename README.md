# Killgroup

## Background

This is a "solution" to the "Chrome didn't shutdown correctly" after starting *Chrome*. It is a "solution" because it works but should be fixed somewhere else, preferably in *systemd*.

## Detailed description

Certain processes termed as "applications" act more like services than applications. These applications include *Chrome* and *Chromium*: even when the application is closed it still leaves processes running in the background. Using the *ps* command reveals that these types of applications have a complex *process tree* and using *killall* ("killall google-chrome" or "killall chromium") doesn't properly kill these applications.

## Solution

These types of applications should be stopped by "nicely" killing only the root process and after that wait for the root process to disappear from /proc. The script *killgroup* does just this: it tries to kill the oldest process with the TERM signal, wait for a predefined time (default max. 10 seconds) and **only if that doesn't work** kill with the KILL signal (forced kill).

## Problems with systemd

There are three problems with *systemd*.

Problem 1:

I reported about the first [original problem](https://bugs.launchpad.net/ubuntu/+source/mosh/+bug/1446982) back in 2015. This problem was related to the *mosh* application. Then I used Ubuntu. Today I mostly use Arch Linux but these problems are probably still valid. The problem I reported back in 2015 was about *systemd* not sending KILL signal to "normal" processes. This can be proved with a simple tester script (here "tester") that writes (and flushes) to a file every time the KILL signal is received. This never happened at system shutdown/restart. I tried this again in 2018 and the same problem persists.

Problem 2:

The problem is caused by the fact that *systemd* doesn't wait for its own shutdown scripts to finish before tearing the processes down. Because of this the *killgroup* script must be run from Gnome shutdown script, from a *bash* alias ('alias myshutdown="killgroup chrome; sudo systemctl poweroff"') or from similar "middleware".

Problem 3:

This problem is the one "fixed" by the *killgroup* script. The *killgroup* script kills processes as trees and waits, unlike *systemd*. It is possible that *systemd* needs some added functionality to accomplish the same.

## Installation

Copy the *killgroup* script to some accessible directory, i.e. /usr/local/bin or similar and make it executable ("cd /usr/local/bin", "sudo chown root:root killgroup", "sudo chmod 755 killgroup"). Then run this script as "killgroup google-chrome", "killgroup chromium" or similar: the name should match the name in the process tree (listed in *ps a* output).

The next step is to call the *killgroup* script from an alias, Gnome script or similar. An example of such a Gnome script is given [here](https://unix.stackexchange.com/questions/49333/how-can-i-run-a-script-during-gnome-logout-if-i-dont-have-admin-privileges).

