#include <stdio.h>
 #include <stdlib.h>
 // Basic Definitions
 #define WHITE 0   //nodes status
 #define GRAY 1
 #define BLACK 2
 #define MAX_NODES 1000   //maximum allowed nodes
 #define INFINITY 1000000000
 /* variable Declarations
 n -number of nodes
e- number of edges
  */
 int n;
int e;
 int capacity[MAX_NODES][MAX_NODES]; // capacity matrix
 int flow[MAX_NODES][MAX_NODES];   // flow matrix
 int color[MAX_NODES]; // store node status
 int parent[MAX_NODES]; // array to store augmenting path
 int min (int x, int y) {
   return x<y ? x : y; // returns minimum of x and y
 }
 // Implementat Queue for BFS
 int head,tail;
 int q[MAX_NODES+2];
 void enqueue (int x) { //add to queue
   q[tail] = x;
   tail++;
   color[x] = GRAY;
 }
 int dequeue () {    //remove from queue
   int x = q[head];
   head++;
   color[x] = BLACK;
   return x;
 }
 // Breadth-First Search for an augmenting path
 int bfs (int start, int target) {
   int u,v;
   for (u=0; u<n+2; u++) { //initialize node status
     color[u] = WHITE;
   }
   head = tail = 0;
   enqueue(start);
   parent[start] = -1;
   while (head!=tail) {
     u = dequeue();
     // Search all adjacent white nodes v. If the capacity
     // from u to v in the residual network is positive,
     // enqueue v.
     for (v=0; v<n+2; v++) {
       if (color[v]==WHITE && capacity[u][v]-flow[u][v]>0) {
         enqueue(v);
         parent[v] = u;
       }
     }
   }
   // If the color of the target node is black now,
   // it means that we reached it.
   //printf("%d\n",color[target]);
   return color[target]==BLACK;
 }
 // Ford-Fulkerson Algorithm
 int max_flow (int source, int sink) {
   int i,j,u;
   // Initialize empty flow.
   int max_flow = 0;
   for (i=0; i<n+2; i++) {
     for (j=0; j<n+2; j++) {
       flow[i][j] = 0;
     }
   }
   // While there exists an augmenting path,
   // increment the flow along this path.
   printf("\nAugmented Path :\n");
   while (bfs(source,sink)) {
     // Determine the amount by which we can increment the flow.
     int increment = INFINITY;
     for (u=sink; parent[u]!=(-1); u=parent[u]) {
       increment = min(increment,capacity[parent[u]][u]-flow[parent[u]][u]);
     }
     //printf("\n%d\n",increment);
     // Now increment the flow.
     for (u=sink; parent[u]!=(-1); u=parent[u]) {
       flow[parent[u]][u] += increment;
       flow[u][parent[u]] -= increment; // Reverse in residual
     }
     printf("\t");
     // Path trace
     for (u=sink; parent[u]!=(-1); u=parent[u]) {
       printf("%d<-",u);
     }
     printf("%d adds %d incremental flow\n",source,increment);
     max_flow += increment;
     //printf("\n\n%d %d\n\n",source,sink);
   }
   //printf("%d\n",s);
   // No augmenting path anymore. We are done.
   return max_flow;
 }
 // Reading the input file and the main program
 int read_input_file() {
   int a,b,i,j;
   char filename[256] = {0};
   printf("Enter data file name : ");
   scanf("%s",&filename);
   FILE* input = fopen(filename,"r");
   if(!input){
     printf("There was a problem opening file %s for reading.\n",filename);
     return 0;
   }
   // read number of nodes and edges
   fscanf(input,"%d %d",&n,&e);
   // initialize empty capacity matrix
   for (i=0; i<n+2; i++) {
     for (j=0; j<n+2; j++) {
       capacity[i][j] = 0;
     }
   }
   // read edge capacities
   for (i=0; i<e; i++) {
     fscanf(input,"%d %d",&a,&b);
     if(a==b){
       printf("ERROR : Graph is not bipartite");
       return 0;
     }
     capacity[a][b]= 1; // Could have parallel edges
   }
   fclose(input);
   for(i=0;i<n/2;i++){
     capacity[0][i+1]= 1;
     capacity[i+6][n+1]= 1;
   }
   return 1;
 }
 int start(){
   int isBipartite=0;
   while(!isBipartite){
     printf("\n*******************************\n\tFind Stable Cuples\n*******************************\n");
     isBipartite=read_input_file();
     //printMatrix();
   }
   return isBipartite;
 }
 int main () {
   int i,j;
   int isBipartite=0;
   isBipartite=start();
   while(isBipartite){
     int choice;
     printf("\n*******************************\n\tFind Stable Cuples\n*******************************\n");
     printf("Enter 1 : Find Optimal Solution\n");
     printf("Enter 2 : Add New Data File\n");
     printf("Enter 0 : Exit\n");
     printf("\nEnter :");
     scanf("%d",&choice);
     if(choice==1){
       printf("\ntotal flow is %d\n",max_flow(0,n+1));
       printf("\nMatched couples:\n");
       for (i=1; i<=n; i++)
           //printf("%d ",parent[i]);
         for (j=1; j<=n; j++)
           if (flow[i][j]>0)
             printf("\tMen:%d matched with Women:%d\n",i,j);
     }else if(choice==2){
       isBipartite=start();
//       printMatrix();
     }
     else if(choice==0){
       printf("Program Terminated\n");
       exit(0);
     }
     else{
       printf("Error : Wrong Input\n");
     }
   }
   return 0;
 }
