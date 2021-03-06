# wifibot
Réalisation d'une application permettant de contrôler le wifibot.

## Ressource
- Wifibot version 3 (documentation : http://www.wifibot.com/wifibot-wifibotlab.html)
- c++
- Qt creator

## Connexion
La première étape est  bien sûr d'établir une connexion avec le robot. Pour que tous commencent sur un bon départ, le code permettant de la mettre en place nous a été fournis.

## Contrôles directionnels
Tout d'abord il est impératif de consulter le [protocol wifi](http://www.wifibot.com/download/2012/Raw_Ethernet_Wifi_protocol_2012.pdf) puisque c'est par cette technologie que la connexion sera maintenue.

Nous allons utilisé du TCP. On va donc envoyer 9 char  
**_Char 1 ->_** = 255  
**_Char 2 ->_** est la taille (ici 0*07)  
**_Char 3-4 ->_** vitesse gauche [0->240 tics max]  
**_Char 5-6 ->_** vitesse droite [0->240 tics max]  
**_Char 7 ->_** détermine si l'on avance ou recule avec cette précision pour chaque côté (droit/gauche)  

### Char 7
Il est composé de 7 bits qui correspond chacun à un charactère spécifique qui doit être préciser.  
**7.** Arrêt des roues du côté GAUCHE  
    * 1-> ON (côté gauche ne roule pas)  
    * 0-> OFF (côté gauche roule)  
**6.** Avant/Arrière GAUCHE  
    ..* 1-> Avant  
    ..* 0-> Arrière  
**5.** Arrêt des roues du côté DROIT  
    ..* 1-> ON (côté droit ne roule pas)  
    ..* 0-> OFF (côté droit roule)  
**4.** Avant/Arrière DROIT  
    ..* 1-> Avant  
    ..* 0-> Arrière  
Les bits de 0 à 3 sont mis à 0  

### Char 8 et 9
il faut le CRC même si dans la doc, il est écrit "not use for TCP" car le wifibot vérifie par le biais du CRC si le message qu'on lui a envoyé n'a pas été endommagé.  
**_Char 8 ->_** les 8 bits bas du CRC -> `astData << 8`
**_Char 9 ->_** les 8 bits bas du CRC -> `castData >> 8`

## Récupérations des données
Cette partie sert à la réception des données que le robot peut nous fournir (niveau de batterie, vitesse, odométrie(nombre de tours éffectué par les roues),...)  
21 octets au total que l'on devra quelque fois convertir en un autre type  
**_bufsend[0-1]->_** vitesse côté droit?  
**_bufsend[2]->_** batterie  
**_bufsend[3-4]->_** infra-rouge avant  
**_bufsend[5-8]->_** odométrie  
**_bufsend[9-10]->_** vitesse côté gauche  
**_bufsend[11-12]->_** infra-rouge arrière  
**_bufsend[13-16]->_** odométrie  
**_bufsend[17]->_** mettre à 0  
**_bufsend[18]->_** version  
**_bufsend[19-20]->_**  permet la vérification du CRC  
