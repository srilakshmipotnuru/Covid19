#project description: 
It keeps track of the record of the number of people being suspected, infected, and recovered, 
considering some probabilities for each phase and the person’s progress to the next phase when the 
required probability is obtained.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_VERTICES 10000 // max number of nodes
#define MAX_EDGES 3000 // max edges per node
#define MAX_EVENTS 100000 //max possible events in priority queue
#define PRECISION 1000 // the precision to which probability is 
calculated
#define TOW 0.5
#define GAMMA 0.2
#define INF 500
#define MAX_TIME 300
struct node // the node represents person
{
int index; // his unique number
char state; // S/I/R state
int pred_time; // the predicted time when it might get infected
struct node* neighbors[MAX_EDGES]; // the array of pointers of 
neighbors
 long int count; // its count of neighbors
struct node *next;
};
struct event
{
int time; // the time of event
char type; // the type of event
struct node *n; // the node corresponding
};
struct event total[MAX_EVENTS]; // array based on heaps
long int event_last=0; // the number of events filled in that array
struct node *s_head,*i_head,*r_head; // the heads and tails of three states of 
people
struct node *s_tail,*i_tail,*r_tail;
long int i_count=0; //infected count
long int r_count=0; //recovered count
int isEmpty(void) //whether array is empty or not
{
if(event_last==0) return(1);
else return(0);
}
void insert(struct event a) // to insert an event
{
/*
basically we insert the node at the end and we check whether the property of heap is 
being followed or not,
that is we repeatedly check whether the child's time value is greater than the parent's 
value and if not we swap them.
*/
total[event_last]=a;
event_last++; // the end value is increased
long int i=event_last-1; // the child
struct event temp;
while(i>0)
{
if(total[i].time<total[(i-1)/2].time) // the parent of ith event is 
(i-1)/2th event
{
temp=total[i];
total[i]=total[(i-1)/2];
total[(i-1)/2]=temp;
i=(i-1)/2; // now the parent 
becomes child and its parent is again (i-1)/2
}
else break; // if the heap 
property is satisfied at some point there is no need to check further up because initially it 
}
// was satisfying and we haven't made changes in that part
}
struct event delete_min(void) // to delete an event
{
if(!isEmpty()) // if it is not empty
{
/*
we just take the 1st node because it is the smallest possible time valued event
we send the last event to 1st place and we make checks to do 
necessary swaps and maintain
heap property.
*/
struct event temp,a;
a=total[0];
total[0]=total[event_last-1];
event_last--;
long int i=0;
long int min=i; // the min refers to the node we are having a 
check at
while(2*i+1<event_last) // we make sure there is at least 
one child for this min or else no need to check
{
// coz it is the leaf
if(total[min].time>total[2*i+1].time)min=2*i+1;
if(2*i+2<event_last&&total[min].time>total[2*i+2].time) min=2*i+2;
// we find the minimum out of parent and its child 
if(i==min) break;
else
{
temp=total[i]; // then we swap it with parent and the check 
continues downwards until a correct place is found for the 
total[i]=total[min];
// the event that has been placed in the 0th position initially
total[min]=temp;
// which was the last element of queue before entering this function
i=min;
}
}
return(a); // finally we return the event with least time
}
else
{
struct event s;
s.time=-1; // in case the queue is empty
}
}
int gettop(void) // to obtain the least timed element of queue
{
if(!isEmpty())
return(total[0].time);
else return(-1);
}
void process_rec_SIR(struct node *x,int time) // the function for recovery of 
node
{
struct node *temp,*temp1;
temp=i_head; // the head of infected list
while(temp->next!=NULL) // we basically remove it from the 
infected list
{
temp1=temp->next;
if(temp1->index==x->index)
{
temp->next=temp1->next;
break;
}
temp=temp->next;
}
x->next=NULL;
r_tail->next=x;
r_tail=r_tail->next; // add it to the recovered list
i_count--; // decrease in infected count
r_count++; // increase in recovered count
x->state='R'; // change of state
}
void process_trans_SIR(struct node *x,int time,float tow,float gamma) // function for 
transmission of infection
{
if(x->state=='S') // we check whether it is susceptible or not
{
struct node *temp,*temp1;
temp=s_head;
while(temp->next!=NULL)
{
temp1=temp->next;
if(temp1->index==x->index)
{
temp->next=temp1->next;
break;
} // remove it from susceptible list
temp=temp->next;
}
x->next=NULL;
i_tail->next=x;
i_tail=i_tail->next; // add it to the infected list
i_count++;
x->state='I'; // change of state
//coin toss
tow=tow*PRECISION;
gamma=gamma*PRECISION; // we multiply the probability 
with precision 
//recovery
int t=time; // time of transmission of infection to this node
while(t<=MAX_TIME) // coz we are concerned of 300 days
{
t++;
if((rand()%PRECISION)<=gamma) // the probability is satisfied only 
then loop breaks
{
struct event temp;
temp.n=x;
temp.time=t;
temp.type='r';
insert(temp); // we add a new event to priority queue
break;
}
}
int recover_time=t; // time of recovery
int i;
for(i=0;i<x->count;i++) // for all the neighbors of this 
node
{
if(x->neighbors[i]->state=='S') // we check if the 
neighbor is susceptible 
{
t=time; // now t refers to the time of transmission
while(t<=recover_time) // assuming only before recovery 
the transmission to neighbors might occur
{
t++;
if((rand()%PRECISION)<=tow) // probability must 
be satisfied for the loop to break
{
if(t<x->neighbors[i]->pred_time) // we add this event in 
case it gets infected earlier due to this source than other source
{
struct event temp;
temp.n=x->neighbors[i];
x->neighbors[i]->pred_time=t; // the pred time is also 
changed
temp.type='t';
temp.time=t;
insert(temp); // new event is inserted
}
break;
}
} // this is done for all neighbors and within 300 days
}
}
}
}
/* MAIN STARTS HERE 
*/
int main()
{
srand(time(NULL));
// all the heads and
tails of 3 lists
s_head=(struct node *)malloc(sizeof(struct node ));
i_head=(struct node *)malloc(sizeof(struct node ));
r_head=(struct node *)malloc(sizeof(struct node ));
s_head->next=NULL;
i_head->next=NULL;
r_head->next=NULL;
s_tail=s_head;
i_tail=i_head;
r_tail=r_head;
struct node* list[MAX_VERTICES]; // the maximum 
vertices represent all the people
struct node *d;
long int i,j,k;
for(i=0;i<MAX_VERTICES;i++) // for each vertex the variable data is 
given
{
list[i]=(struct node *)malloc(sizeof(struct node));
list[i]->index=i;
list[i]->count=0;
list[i]->next=NULL;
list[i]->state='S';
list[i]->pred_time=INF; //just some large value than MAX_TIME
d=list[i]; // and all are added to susceptible list
s_tail->next=d;
s_tail=s_tail->next;
}
for(i=0;i<MAX_VERTICES;i++) // for each vertex the 
neighbors are given(undirected)
{
if(list[i]->count<MAX_EDGES) // we check if the 
count of neighbors hasn't reached limit
{
long int edges=rand()%(MAX_EDGES-list[i]->count); // 
with the remaining neighbors that could be filled we find random number
for(j=0;j<edges;)
{
long int n=rand()%MAX_VERTICES; // some 
random vertex is selected
int check=1; // which makes sure that no problem is 
there in creating edge 
for(k=0;k<list[i]->count;k++)
{
if(list[i]->neighbors[k]->index==n)
{
check=0; // we ensure that 
this vertex is not already connected to it coz we don't want parallel edges
break;
}
}
if(list[n]->count>=MAX_EDGES) check=0; // if 
the count has reached its upper limit 
if(check==1&&n!=i) // we ensure that a self 
edge doesn't occur 
{
list[i]->neighbors[list[i]->count++]=list[n]; // since undirected graph 
we change the neighbor's list of both
list[n]->neighbors[list[n]->count++]=list[i];
j++; // this edge 
was made so we increment for next edge
}
}
}
}
for(i=0;i<MAX_VERTICES;i++)
{
if(rand()%2==1) // we randomly find one vertex 
and then this event is added to queue
{
int time=1;
struct event temp;
temp.n=list[i];
list[i]->pred_time=1;
temp.time=time;
temp.type='t';
insert(temp);
break;
}
}
int day; // the day we are in
int top; // the upcoming event's time
for(day=1;day<=MAX_TIME;day++)
{
top=gettop(); // we find the least time event
while(top!=-1&&top==day) // we ensure that the day 
has reached for this event to occur and also that queue is not empty
{
struct event temp=delete_min(); // we delete it from queue
if(temp.type=='t')
{
if(temp.n->state=='S')
{
process_trans_SIR(temp.n,temp.time,TOW,GAMMA);
// function call
}
}
else if(temp.type=='r')
{
process_rec_SIR(temp.n,temp.time); // 
function call
}
top=gettop(); // we do this until all 
scheduled events on this day are done
}
printf("%d,%ld,%ld\n",day,i_count,r_count); // the day, 
infected list count, recovered list count
}
return(0); // end of code
}
DSAProj.c
Displaying DSAProj.c.