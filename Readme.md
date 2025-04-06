# Doodle Jump Clone

Dieses Projekt ist ein Doodle Jump Klon, der in C++ unter Verwendung der SFML-Bibliothek entwickelt wurde. Ziel des Projekts ist es, zu demonstrieren, dass man eine vollständige Desktopanwendung von der Idee bis zur fertigen Ausführung umsetzen kann – inklusive Einbindung externer Bibliotheken, sauberer Code-Strukturierung in Header- und Quell-Dateien, Implementierung dynamischer Algorithmen und Spielmechaniken sowie der Nutzung eines Build-Systems (CMake).

---


## Projektinhalt

- **Quell- und Header-Dateien:**  
  Alle Dateien (z. B. `Game.cpp`, `Game.hpp`, `Player.cpp`, `Player.hpp`, `Platform.cpp`, `Platform.hpp`, `Enemy.cpp`, `Enemy.hpp`, `Boost.cpp`, `Boost.hpp`, `main.cpp`) sind im Projekt enthalten.

- **Assets:**  
  Alle benötigten Bilder sind enthalten, darunter:
    - Hintergrundbild: `background.png`
    - Spielerbilder: `doodle_right.png`, `doodle_left.png`
    - Boost-Bild: `boost.png`
    - Weitere Bilder für Gegner, Plattformen und ggf. Sounds
    - `highscore.txt` wird zur Speicherung des Highscores genutzt (wird automatisch erstellt/aktualisiert).

- **Build:**  
  Das Projekt wird mittels CMake gebaut. Eine ausführbare Datei wird für mindestens eine Plattform (z. B. Windows) erzeugt.

- **Dokumentation:**  
  Dieses README erläutert den Aufbau des Projekts, die implementierten Funktionen und die verwendeten Drittleistungen.

---

## Spielmechanik und Besonderheiten

- **Dynamische Plattformgenerierung:**  
  Plattformen werden in zufälligen Abständen generiert, sodass der Spieler kontinuierlich neue Herausforderungen hat.

- **Gegner und Boosts:**  
  Gegner und Boosts werden dynamisch erzeugt.
    - **Boosts:** Erscheinen ausschließlich auf statischen Plattformen (nicht bewegend) im Sichtfeld als "Sprungfedern" – beim Einsammeln erhält der Spieler einen verstärkten Sprung.
    - **Gegner:** Werden ebenfalls dynamisch generiert und führen bei einer Kollision zum Game Over.

- **Kameraflug:**  
  Die Kamera folgt dem Spieler, sodass immer der relevante Spielbereich sichtbar ist.

- **Hintergrundbild:**  
  Ein fixiertes Hintergrundbild (`background.png`) wird als erster Layer im render()-Block gezeichnet, sodass es den gesamten Bildschirm abdeckt.

- **Highscore-Speicherung:**  
  Der Highscore wird in der Datei `highscore.txt` gespeichert und beim Spielstart geladen, sodass er auch über Neustarts hinweg erhalten bleibt.

---

## Steuerung

- **Linke/Rechte Pfeiltaste:**  
  Bewege den Spieler nach links bzw. rechts. Der Charakter spiegelt sich automatisch, sodass er in Bewegungsrichtung blickt.

- **ESC:**  
  Schaltet das Pausenmenü ein oder aus.

- **R:**  
  Startet das Spiel nach einem Game Over neu.

- **Automatischer Sprung:**  
  Der Spieler springt automatisch, wenn er eine Plattform berührt; beim Einsammeln eines Boosts wird ein verstärkter Sprung ausgelöst.

---

## Dateistruktur

Die Projektstruktur ist wie folgt organisiert:

- **Doodle_Jump_Clone/**
    - **cmake-build-debug/**  
      Enthält die generierte ausführbare EXE, Build-Artefakte und den Ordner **assets/**, in dem alle benötigten Assets (Bilder, Sounds, etc.) abgelegt sind.
    - **src/**  
      Enthält alle Quell-Dateien (.cpp) und den Unterordner **headers/**:
        - `main.cpp`
        - `Game.cpp`
        - `Player.cpp`
        - `Platform.cpp`
        - `Enemy.cpp`
        - `Boost.cpp`
        - **headers/**  
          Enthält alle Header-Dateien:
            - `Game.hpp`
            - `Player.hpp`
            - `Platform.hpp`
            - `Enemy.hpp`
            - `Boost.hpp`

    

---

## Zusammenfassung

Dieses Doodle Jump Clone-Projekt demonstriert, wie man in C++ eine vollständige Desktopanwendung mit SFML umsetzt. Wichtige Aspekte des Projekts:

- **Einbindung externer Bibliotheken:**  
  Nutzung von SFML für Grafik, Audio, Eingaben und Fensterverwaltung.

- **Dynamische Spielmechanik:**  
  Implementierung von Plattform-, Gegner- und Boost-Generierung, Kollisionsprüfungen und Kamera-Scrolling.

- **Highscore-Speicherung:**  
  Der Highscore wird in einer Datei gespeichert und über Spielneustarts hinweg geladen.

- **Saubere Code-Struktur:**  
  Aufteilung in Header- und Quell-Dateien sowie Einsatz von CMake für den Build-Prozess.

---

## Ausführung

Um das Spiel zu starten, führe die erzeugte ausführbare Datei im Build-Ordner (z. B. `cmake-build-debug`) aus. Achte darauf, dass alle erforderlichen Assets (Bilder, Schriftarten etc.) relativ zur ausführbaren Datei im Ordner `assets` vorhanden sind.

---

## Kontakt

Bei Fragen oder Feedback kannst du dich gerne per E-Mail an ben.gnielka@icloud.com wenden.
