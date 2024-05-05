#!/bin/bash

# Récup PID
pid=$(ps aux | grep '[k]eylogger' | awk '{print $2}')

# Vérif
if [ -z "$pid" ]; then
    echo "Le daemon keylogger n'est pas en cours d'exécution."
else
    echo "Arrêt du daemon keylogger avec le PID $pid."
    kill $pid
    
    wait $pid 2>/dev/null
    echo "Daemon keylogger arrêté."
fi