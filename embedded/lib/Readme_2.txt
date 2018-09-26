Dieses Dokument soll die Installation und den Gebrauch für das FTS der Firma Dropkick erläutern

Programmierumgebung: 
Der Code wurde auf Plattform IO in Microsoft Visual Studio Code geschrieben.
Die Anleitung ist folglich an diese Programmierumgebung angepasst.

Configuration:
Das Configuration-file enthält die wichtigen Parameter zum Einstellen von Pins, Reglerwerten, etc.

Debugger:
Innerhalb des Configuration-file existiert der Boolean DEBUGGER.
wird dieser auf true gesetzt, so werden die Serial ausgaben an den Monitor aktiviert.

Libraries:
Folgende Libraries müssen dem System bekannt sein:
WiFiClient.h
SPI.h
Servo.h
Adafruit Motor Shiel V2 Library
New Ping.h
ArduinoJson.h
PixyI2C.h
Wire.h

Die Library PixyI2C kann als Zip.file von der Homepage der PixyCam runtergeladen werden.
Die restlichen Libraries sind direkt in Plattform IO erhältlich

Das Fahrzeug besteht aus folgenden Klassen/Modulen:
Chassis
Hoist
Sonar
Vision
VehicleWebAPI

Alle Klassen sind von der Basisklasse Modular abgeleitet.
Die Klasse VehicleWebAPI ist ihrerseits von der Klasse WebAPI abgeleitet.

Die Logik des Fahrzeuges läuft in der Strategie und arbeitet mit den Loops der einzelnen Module zusammen.
Structs dienen dem Programm zur Übertragung von Werten zwischen dem main und den Modulen.
