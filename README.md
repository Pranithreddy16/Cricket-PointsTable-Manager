# Cricket-PointsTable-Manager
## Project Overview
This project is a console-based C application developed to manage a cricket tournament in a structured manner. It allows users to create teams, generate round-robin fixtures, record match results, and display the points table. The application uses file handling to save tournament progress so that data is preserved across multiple executions.

## Workflow
- Program starts by checking for previously saved tournament data.
- If data exists, it loads teams, fixtures, and results from file.
- If no data is found, the user creates teams and fixtures are generated.
- User interacts with the system through a menu to update matches or view details.
- Tournament state is saved when the program exits.

## Core Concepts Used
- Structures to represent teams and match fixtures.
- Arrays for storing multiple teams and matches.
- Functions for modular and readable code.
- Sorting to rank teams based on points.
- File handling for saving and loading data.

## Features
- Automatic generation of round-robin fixtures.
- Menu-driven interface for easy interaction.
- Match result updating with validation.
- Dynamic points table generation.
- Consistent storage using text files.

## File Handling
- Uses a text file to store tournament data.
- Saves team statistics and match results.
- Loads saved data on program startup.
- Ensures continuity of tournament progress.

## Tech Stack
- Language:
   - C
- Libraries:
   - stdio.h
   - stdlib.h
   - string.h
   - time.h
- Interface:
   - Console-based application

## Author
Pranith Reddy Donthireddy
