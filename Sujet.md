# TP 5 et 6 : Projet

Ce projet se réalise en binôme sur une carte WIO Terminal qui ne pourra pas vous être prêtée en dehors de vos 2 créneaux !

## 1 - Contexte et sujet

Votre société vous demande de réaliser un prototype le plus réaliste et le plus fluide possible sur un WIO Terminal. Ce prototype utilisé par un vigile à l'entrée d'un petit établissement recevant du public (ERP niveau 5) permet de :
- comptabiliser les gens qui entrent et qui sortent ;
- afficher sur l'écran du WIO le nombre de visiteurs (cumulés), ainsi que l'affluence, c'est-à-dire le nombre de visiteurs présent à l'instant t ;
- remettre à zéro les compteurs ;
- afficher l'heure sur l'écran du WIO ;
- envoyer toutes les 15 minutes par communication série, les données essentielles ( l'heure, le nombre de visiteurs (cumulés) et l'affluence) ;
- être prévenu qu'un message a été reçu sur la communication série ;
- lire un message précédemment reçu.

## 2 - Cahier des charges

On vous demande :
- d'initialiser une communication série à 115200 bauds et de veiller dans votre code à ne pas avancer dans l'application tant que cette communication n'est pas opérationnelle ;
- de comptabiliser une entrée dans l'établissement via le bouton C (qui incrémente donc de 1 le compteur de visiteurs et le compteur d'affluence) ;
- de comptabiliser une sortie de l'établissement via le bouton B (qui décrémente de 1 l'affluence) ;
- d'afficher un bandeau bleu en haut de l'écran précisant l'usage des boutons (+ - RAZ en-dessous des boutons C B et A) et donnant l'heure au format HH:MM le plus à droite possible ;
- d'afficher en-dessous sur fond vert le nombre de visiteurs et l'affluence. Si l'affluence dépasse l'affluence max acceptable dans l'établissement, un bip se fait entendre et le fond vert passe au rouge. Le fond reste rouge tant que l'affluence est supérieure à l'affluence max. Le fond redevient vert dès que l'affluence est inférieure ou égale à l'affluence max. Le bip a lieu uniquement lors du passage du fond vert au fond rouge. L'affluence max pour ce premier prototype est une constante de votre programmable que vous fixerez à la valeur 10 dans votre code ;
- d'initier une remise à zéro des compteurs via le bouton A qui a pour conséquence de demander, en bas à droite de l'écran, à l'utilisateur de confirmer cette remise à zéro ; confirmation qui doit intervenir dans les 5 secondes via une action PRESS sur le joystick. Au-delà des 5 secondes, le message disparait et l'action PRESS n'a plus d'effet. Si besoin, il faut donc recommencer par un enfoncement du bouton A. Si la confirmation a lieu dans les 5 secondes, outre la remise à zéro des compteurs précédents et des affichages associés, l'heure et l'information de la RAZ sont envoyées sur la communication série. A noter que, pendant l'affichage de confirmation, il est impossible d’accéder à l'écran des messages (expliqué plus bas) mais il est toujours possible de comptabiliser une entrée et/ou une sortie qui a pour conséquence d'actualiser les affichages sur l'écran ;
- d'afficher sur votre écran, juste en-dessous de l'affluence, une information indiquant l'arrivée d'un message sur le port série de votre WIO. Votre WIO joue alors une courte mélodie qui ne peut être confondue avec le bip d'alerte sur l'affluence. Tant que le message n'est pas lu, cette information reste afficher sur votre écran. Les messages sont peu fréquents mais peuvent être très importants ; l'alerte doit donc être quasi concomitante à l'arrivée du message ;
- de lire un message précédemment arrivé, même si il a été déjà lu, avec l'action UP du joystick. L'écran passe alors entièrement sur fond blanc et présente le message qui peut être assez long, ainsi qu'un texte qui indique à l'utilisateur d'utiliser l'action DOWN du joystick pour sortir du menu et rejoindre l'écran précédent (écran principal). Sans action de l’utilisateur, on sort et rejoint l'écran principal au bout de 30 secondes. Les actions des autres boutons sont sans aucun effet tant que l'on est dans l'écran de lecture des messages. Un nouveau message écrase le message précédent. Il est impossible d'aller vers l'écran des messages si il n'y a pas eu un premier message ;
- d'envoyer sur la communication série quand les minutes de l'horloge sont égales à 00 15 30 et 45, une ligne de tirets suivie des données suivantes : l'heure au format HH:MM, le nombre de visiteurs et l'affluence (une information par ligne) ;
- de veiller à ce que les actions sur les boutons A B C et PRESS (RAZ - + et ValidationRAZ) aient un effet quasi instantanée sur les affichages afin de veiller à une bonne fluidité de l'application. Lors de l'arrivée de groupes de personnes, le vigile doit pouvoir appuyer rapidement sur le bouton A pour les prendre en compte. Il en est de même pour les sorties massives de gens ;
- d'utiliser une police de taille 3 ;
- de bien structurer et commenter votre code.
