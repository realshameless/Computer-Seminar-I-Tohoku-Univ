# Computer-Seminar-I-Tohoku-Univ.
Hello World! 
i just finish my computer seminar I final project 2 weeks ago, I want to publish my code as my first project on github. I hope it would somehow help you to pass the class.

the map data in this sample code suppose to be (int)crossing_number\n(int)id[n],(double)position_x[n],(double)position_y[n],(int)wait_time[n],(char)japanese_name[n],(char)english_name[n],(int)points[n],(int)next[Point_number][n] n is the number of crossing numbers

In the first version, i use Dijkstra algrithom to search shortest path, I added few features in order to meet the requirement:

1) Map zoom in/out, move to left/right/up/down
2) Marker is displayed as a triangle symbol that points to the direction it is moving to
3) Intermediate points of a route can be specifie (80 stopovers maximum)
4) You can save your search history for 1 time with a ASCII based password protection. Later, if you input the password, you could read from the saved history.

in the future, i am planning to improve my code with following functions: 

1) Using A* algrithom to replace Dijkstra, because Dijkstra would search distance for every crossing in the map, in the real world, the map data would be huge and impossible to search all of them. A* is a better way to do it. 
2) I am going to use AES enctryption to replace ASCII based simple arithmetic encryption 
3) using python or Tensorflow to build a simple neural network to trick few words, such as yes/yeah/yep/sure/of course..etc. 
4) access online map API to gain real time traffic info, and influence the speed of the car in the map.

so, stay tuned folks...
