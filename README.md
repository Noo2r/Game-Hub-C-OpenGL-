# 🎮 Game Hub

**A C++ OpenGL-based Collection of Classic Games with a Fun Twist**

---

## 🧩 Overview

Welcome to the **Game Hub**, a colorful and charming bundle of retro games recreated using modern C++ and OpenGL!  
This delightful project includes four bite-sized games—each with a sprinkle of humor, unique character textures, and snappy audio.

---

## 📦 Included Games

### 🏓 Pong: Zoro vs. Luffy Edition
**File:** `pong.cpp`

#### Features:
- 2-player paddle action with anime flair (Zoro, Luffy, Buggy)
- Bouncing ball with realistic reflection angles
- Textured characters and dynamic scoring
- Cool background music and retro vibes

#### Controls:
- Player 1: `W` / `S`
- Player 2: `↑` / `↓`
- `ESC` to return to main menu

---

### ❌⭕ Tic Tac Toe
**File:** `xo.cpp`

#### Features:
- 2-player classic grid war
- Crisp UI with smooth mouse interaction
- Win tracking and a snappy restart button
- Victory and draw detection logic
- Background music for an immersive twist

#### Controls:
- Mouse click to play
- Click "Restart" to go again!

---

### 🐦 Flappy Bird Clone
**File:** `flappy.cpp`

#### Features:
- One-button madness: jump through pipes!
- Auto-scrolling background and gravity physics
- Live score + persistent high score system
- "Game Over" restart and return menu
- Whistle-worthy background soundtrack

#### Controls:
- Press `Space` to flap
- Mouse click buttons on game over screen

---

### 🧠 Memory Match
**File:** `memory_game.cpp`

#### Features:
- 2-player matching game with score tracking
- Click to reveal cards and test your memory
- Easy and Hard mode with adjustable grid
- Color-coded card ownership and clean layout
- Thoughtful delay logic on mismatched pairs
- Tranquil background music

#### Controls:
- Mouse click to flip cards
- Choose difficulty from menu

---

## 🔊 Audio Support

Each game includes its own themed `.wav` file to set the mood:
- `joyboy.wav`
- `xo.wav`
- `flappybird.wav`
- `memory.wav`
- `menu.wav`

> Note: Audio uses `PlaySound` from `mmsystem.h`, so it's **Windows-only**.

---

## 💡 Technical Highlights

- Modular design with namespaces for each game
- GLUT used for rendering and input handling
- Texture loading via `stb_image` (used in Pong)
- Collision detection and basic physics in Flappy and Pong
- Clean game loop logic using `glutTimerFunc` and `glutPostRedisplay`

---

## 🛠 Setup Requirements

- OpenGL and GLUT libraries
- Windows platform (due to audio and window dependencies)
- Linker flags: `-lopengl32 -lglu32 -lfreeglut -lwinmm`
  
---

## 🚀 How to Run

Compile each game separately using a C++ compiler configured for OpenGL and Windows audio support.  
Ensure all `.cpp`, `.wav`, and texture assets are in the project directory.

```bash
g++ pong.cpp -o pong.exe -lopengl32 -lglu32 -lfreeglut -lwinmm
