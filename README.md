# A Knowledge-based AI Agent
My agent class is implemented in MyAI.cpp and MyAI.hpp. 
I use a two dimensional board vector to maintain the state at each position. The agent senses the environment around its position when it is moving forward, and updates the knowledge base. It keeps track of where it’s already been and which next step is safe to go. When it senses stench or breeze, then the next cell could be dangerous and it turns back and returns to the starting position using breadth first search algorithm.
