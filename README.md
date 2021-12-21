# Robot Evolution Simulator

This program simulates evolution in a population of 200 robots. Robots are generated with random genes that dictate their response to sensor readings from their surroundings.
Each robot is placed on the map and allowed to move as many times as the user specified. Running into obstacles drains energy while running into battery cells increases energy.
After each of the 200 robots has been tested on the map, the top 100 robots reproduce with each other to create 100 new robots. The new robots inherit their genes from their
parents with a 50-50 probability for each gene and a small chance of mutation. Each new population is then tested on the map for the specified number of generations and the average
energy levels at the end are outputted.  
  

Robots with good genes will move towards batteries and away from obstacles. Because these robots will score the highest, their genes will dominate the gene pool over time and the average
energy levels of the population will increase.  
  
Test it here: https://replit.com/@Jaymett/Robot-Evolution#main.cpp

