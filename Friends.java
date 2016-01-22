//Terrence Williams
//Matthew Kownacki
import java.lang.*;
import java.util.*;
import java.io.*;



//This is the object we will use to hold most of our data of our individuals that are going in to the graph.
class Person{
	String name;
	String school;
	int vertNum;
	ArrayList<Person> frands;
	public Person(String namePerson, String schoolName, int vertn){
		name=namePerson;
		school=schoolName;
		vertNum=vertn;
		frands= new ArrayList(10);
	}}

	//This is the Person Node for the linked lists we will build later.
class PNode{
		PNode prev;
		PNode next;
		Person nodeData;
		public PNode(PNode p1, PNode p2, Person enter){
			prev=p1;
			next=p2;
			nodeData=enter;
		}
	}

//This is the interface for the assignment. It isn't terribly flashy, and while I have some sympathy for the user, I will make a disclaimer that
// any frustration come about from mistyping inputs is their own fault. 
public class Friends{
	

	public static void main(String[] args) throws FileNotFoundException{
		System.out.println("Please enter the graph file:");
		Scanner mc= new Scanner(System.in);
		String filename=mc.nextLine();
	PNode[] graph=BuildGraph(filename);
	   String option="";
	   ArrayList<Person> MasterPersonList= new ArrayList(10);
	   for(int j=0; j<graph.length; j++){
		   MasterPersonList.add(graph[j].nodeData);
		   
	   }
	   while(option.equals("quit")!=true){
		   System.out.println("What would you like to do now? (ENTER THE INTEGER CORRESPONDING TO YOUR OPTION CHOICE)");
		  
		   System.out.println("1. Shortest intro chain.");
		   System.out.println("2. Cliques at a school.");
		   System.out.println("3. Connectors.");
		   System.out.println("4. Quit.");
		  
		   for(int i=0; i<graph.length; i++){
		//   System.out.println(i + "th PNOde: ");
		   PNode ptr=graph[i];
		   while(ptr.next!=null){
			 //  System.out.println(ptr.nodeData.name);
			   ptr=ptr.next;
		   }
	//	   System.out.println(ptr.nodeData.name);
		   }
		  	   option=mc.nextLine();
		  	   if(option.equals("1")){ boolean screwedUp=true;
		  	 String name1=""; String name2="";
		  		   		  	   while(screwedUp){
		  		   System.out.println("Input 2 people:");
		  		  name1=mc.nextLine();
		  		  name2=mc.nextLine(); boolean n1=false; boolean n2 = false;
		  		  name1=name1.toLowerCase(); name2=name2.toLowerCase();
		  		   for(int j=0;j<MasterPersonList.size();j++){
		  			   if(name1.equalsIgnoreCase(MasterPersonList.get(j).name)){n1=true;}
		  			 if(name2.equalsIgnoreCase(MasterPersonList.get(j).name)){n2=true;}
		  			   
		  		   }
		  		   if(n2 ==false || n1 ==false){ System.out.println("Please Enter people who are in the graph:");continue;}
		  		   else{screwedUp=false; break;}}
		  		   String result=ShortestPath(name1,name2,graph);
		  		   System.out.println("Your result is:");
		  		   System.out.println(result);
		  		   System.out.println("Press enter to continue: ");
		  		   option=mc.nextLine();
		  		   
		  		   
		  		   
		  	   }
		  	   if(option.equals("2")){ boolean ScrewedUp=true;
		  	 String schoolname=""; boolean s1=false;
		  		   while(ScrewedUp){
		  		   System.out.println("Input a school name:");
		  		  schoolname=mc.nextLine(); 
		  		  schoolname=schoolname.toLowerCase();
		  		for(int j=0;j<MasterPersonList.size();j++){
		  			if(schoolname.equalsIgnoreCase(MasterPersonList.get(j).school)){
		  				s1=true;
		  			}
		  			
		  		}
		  		   if(s1==false){ System.out.println("Please input a correct school name:"); continue;}
		  		   else{ ScrewedUp=true; break;}
		  		   }
		  		   ArrayList<PNode[]> Cliques= Cliques(schoolname,graph);
		  		   if(Cliques.isEmpty()){System.out.println("No Cliques"); continue;}
		  		  System.out.println("The cliques are as follows:");
		  		   for(int i=0;i<Cliques.size();i++){
		  			   System.out.println("Clique " + i + ":");
		  		   PNode[] temp= Cliques.get(i);
		  	//	   System.out.println("LINE 82");
		  		 for(int j=0;j<temp.length;j++){
		  		  // System.out.println(j + "th PNOde: ");
		  		   PNode ptr=temp[j];
		  		   System.out.println(ptr.nodeData.name);
		  		//   while(ptr.next!=null){
		  			//   System.out.println(ptr.nodeData.name);
		  			  // ptr=ptr.next;
		  		   //}
		  	//	   System.out.println(ptr.nodeData.name);
		  		   } System.out.println();}
		  		 System.out.println("Press enter to continue: ");
		  		   option=mc.nextLine();
		  		  }  
		  	  if(option.equals("3")){
		  		  System.out.println("THE CONNECTORS OF THIS GRAPH ARE:");
		  		  ArrayList<String> connects= listConnector(graph);
		  		  for(int zunno=0;zunno<connects.size();zunno++){
		  			  if(zunno!=connects.size()-1){
		  			  System.out.print(connects.get(zunno) + ", ");}
		  			  else{System.out.print(connects.get(zunno));}
		  		  }
		  		  System.out.println();
		  		System.out.println("Press enter to continue: ");
		  		   option=mc.nextLine();
		  	  }
		  	   if(option.equals("4")){System.exit(1);}
		  	   
		  	   
		  	   
		   
		  	   }
	   }
	
	

//This builds the graph using a file input, and a host of helper methods listed and documented below.
public static PNode[] BuildGraph(String fileinput)
	throws FileNotFoundException {
	String eol=System.getProperty("line.separator");
	ArrayList<String> lineList= new ArrayList(10);
	ArrayList<Person> peopleList= new ArrayList(10);
		Scanner sc = new Scanner(new File(fileinput)).useDelimiter(eol);
		while (sc.hasNext()) {
			String nextline = sc.next();
			nextline=nextline.toLowerCase();
			if(! lineList.contains(nextline)){lineList.add(nextline);}
		}
	//	System.out.println(lineList);
		int numPeople= Integer.parseInt(lineList.get(0).replaceAll(" ",""));
		lineList.remove(0);
	//	System.out.println("Line 75"); System.out.println(lineList);
		for(int i=0;i<numPeople;i++){
			populateList(lineList.get(i),peopleList,i);
		} int j=0;
		while(j<numPeople){
			lineList.remove(0);
			j++;
		}
	
		
		PNode[] sandwich= new PNode[numPeople];
		//System.out.println("Line 88");
		for(int k=0; k<peopleList.size();k++){
			PNode alpha=new PNode(null, null,peopleList.get(k));
			sandwich[k]=alpha;
		}
	//	System.out.println("Line 93");
	LinkedListFill(sandwich,lineList,peopleList);
	//System.out.println("line 95");
	MirrorFriends(sandwich);
	return sandwich;
}


//This populates the list of "People" from the lines taken from the main file.
public static void populateList(String line, ArrayList<Person> peopleList, int vertnum){
	 ArrayList<String> tempSpace= new ArrayList(5);
		StringTokenizer hero=new StringTokenizer(line, "|");
		Person newguy;
		while(hero.hasMoreTokens()){
			tempSpace.add(hero.nextToken());
		}
		if(! tempSpace.isEmpty()){
			if(tempSpace.size()>1){
		if(tempSpace.get(1).equals("y")){
		 newguy= new Person(tempSpace.get(0),tempSpace.get(2),vertnum);
		}
		else{ newguy=new Person(tempSpace.get(0),"",vertnum);
		}
		peopleList.add(newguy);}
		}
}



//This fills the linked list from the given ArrayLists of Strings and Persons that we took from the file input.
public static void LinkedListFill(PNode[] sandwich, ArrayList<String> friendList, ArrayList<Person> peopleList){
	while(! friendList.isEmpty()){
	StringTokenizer hero=new StringTokenizer(friendList.get(0), "|");
	ArrayList<String> tempSpace= new ArrayList(3);
	while(hero.hasMoreTokens()){
		tempSpace.add(hero.nextToken());
	}
if(! tempSpace.isEmpty()){
	Person dude1=peopleList.get(0); Person dude2=peopleList.get(0);
	boolean change=false;
	String person1= tempSpace.get(0);
	String person2=tempSpace.get(1);
	for(int k=0;k<peopleList.size();k++){
		if(person1.equals(peopleList.get(k).name)){
			dude1=peopleList.get(k);
			
		}
		if(person2.equals(peopleList.get(k).name)){
			dude2=peopleList.get(k);		
			
		}}
		
		TraverseSandwich(sandwich,dude1.vertNum,dude2);
		}
	
	friendList.remove(0);
	}
}
//This method traverses the linked list to add a new entry to the graph.
	public static void TraverseSandwich(PNode[] sandwich, int vertnum,Person guy){
		PNode ptr= sandwich[vertnum];
		ptr.nodeData.frands.add(guy);
		PNode addition= new PNode(null,null,guy);
	while(ptr.next !=null){
			ptr=ptr.next;
		}
		ptr.next=addition;
		addition.prev=ptr;

}
//Clearly if you are friends with somebody, they should be friends with you as well. So this takes care of the two way nature of this relationship.
	
//Note: this does not effect the linked list implementation, it only updates each person's list of people they are friends with overall.
	//Thus the number of edges in the graph are not counted twice.
public static void MirrorFriends(PNode[] graph){
	for(int i=0;i<graph.length;i++){
		for(int j=0;j<graph.length;j++){
			if(graph[j].nodeData.frands.contains(graph[i].nodeData)){
				graph[i].nodeData.frands.add(graph[j].nodeData);
			}
		}
	}
	
}
//This is the method that takes a name and turns it into a vertex number.
public static int Name2Vert(String name,PNode[] sandwich){int vertnum=-1;
		for(int j=0;j<sandwich.length;j++){
			if(sandwich[j].nodeData.name.equalsIgnoreCase(name)){vertnum=j;}
		}
		return vertnum;
	}
//This method translates a vertex number into a name. 
public static String vert2Name(int vertnum, PNode[] sandwich){String name= "";
	
name=sandwich[vertnum].nodeData.name;
return name;
}

//THUS BEGINS ALGORITHM 1 
//This is the Shortest Path algorithm that will be utilized extensively throughout the rest of the assignment.
//It executes between Person A and Person B, going recursively through all of Person A's friends to B, Person A's friends' friends to B, and so on.
//Then it matches up all possible existing paths, and picks the one of shortest length ( shortest "jumps", a jump is indicated by "-->" ).
public static String ShortestPath(String name1, String name2, PNode[] graph){String target="";
if(name1.equals(name2)){return "Very funny. That person clearly can reach themselves just fine without friends.";}
//System.out.println(name1);
//System.out.println(name2);
	int vert1=Name2Vert(name1,graph); if(vert1==-1){return "not reachable by this person";}Person p1= graph[vert1].nodeData;
	if(isFriend(name1,name2,graph)){target= name1 + "-->" + name2; return target;}
	ArrayList<String> pathholder=new ArrayList(10);
	PNode[] graphnew= new PNode[graph.length-1];
	int k=0;int b=0;
			
				while(b<graphnew.length){
					if(k!=vert1){graphnew[b]=graph[k]; k++; b++; continue;}
					k++;
				}
			
	
	
	
	for(int i=0;i<p1.frands.size();i++){
	//	System.out.println(name1 + " " + name2);
		Person nextone =p1.frands.get(i);
		
	String placeholder=	name1.concat("-->").concat(ShortestPath(p1.frands.get(i).name,name2,graphnew));
	if(! placeholder.isEmpty()){
	pathholder.add(placeholder);}
	}
	int size=pathholder.get(0).length(); int winner=0; boolean safety=false;
	for(int g=0;g<pathholder.size();g++){
		if(pathholder.get(g).lastIndexOf("not reachable by this person")==-1){safety=true;}
	}
	for(int j=0; j<pathholder.size(); j++){
		if(safety){ if(pathholder.get(j).lastIndexOf("not reachable by this person")!=-1){continue;}}
		String[]a=pathholder.get(j).split("-->");
		if(a.length<size){winner=j; size=a.length;}
	}
	if(! pathholder.isEmpty()){
	target=pathholder.get(winner);} 
	//if(target.lastIndexOf("not reachable by this person")!=-1){target="";}
	return target;
}





 public static boolean isFriend(String name1, String name2, PNode[] graph){ boolean result=false;
int vert1=Name2Vert(name1,graph);	
int vert2=Name2Vert(name2,graph);
if(vert1==-1 || vert2==-1){return false;}
Person p1=graph[vert1].nodeData;
Person p2=graph[vert2].nodeData;
if(p1.frands.contains(p2)){result=true;}
if(p2.frands.contains(p1)){result =true;}
	 return result;
 }

//ALGORITHM 2 BEGINS HERE
 //This will utilize the Shortest Path algorithm and recursively build new "cliques" using the schoolname. 
 //The graph knows to put who in which island when the Shortest Path algorithm returns a "no possible path" between groups of people. 
 //It then feeds the distinct groups of people into the original graphbuilding algorithm from above after formatting the strings correctly.
 //That is essentially all that this method is doing in a nutshell.
 public static ArrayList<PNode[]> Cliques(String schoolname, PNode[] graph){
	 
	 ArrayList<ArrayList<String>> Cliques= new ArrayList(10);
	 
	 ArrayList<Person> Names= new ArrayList(10);
	 
	 
	 
	 
	 
	 for(int j=0; j<graph.length; j++){
		 
		 if(graph[j].nodeData.school.equals(schoolname)){
			 Names.add(graph[j].nodeData);
		 }
	 }
	 PNode[] newgraph= new PNode[Names.size()];
	 int k=0;int b=0;
		
		while(b<newgraph.length){
			if(graph[k].nodeData.school.equals(schoolname)){
				
				
				newgraph[b]=graph[k]; k++; b++; continue;}
			k++;
		}
		 
		ArrayList<PNode[]> GraphZ=new ArrayList(10);
		 
		 
//	 System.out.println("Line 287");
	for(int alpha=0; alpha<Names.size(); alpha++){
		
	//	System.out.println(Names.get(alpha).name);
	}
//	System.out.println("Line 292");
	
	 while(! Names.isEmpty()){
		 ArrayList<String> Clique=new ArrayList(10);
		 for(int i=0;i<Names.size();i++){
		 if(! Clique.contains(Names.get(0).name)){
		Clique.add(Names.get(0).name);}
		 
			if(ShortestPath(Names.get(0).name,Names.get(i).name,newgraph).lastIndexOf("not reachable by this person")==-1){
				if(! Clique.contains(Names.get(i).name)){
				Clique.add(Names.get(i).name);}
			}
				
			
		
		 }
	//	 System.out.println("Line 298");
	//	 System.out.println("Cliques are: " + Clique);
		 if( ! Cliques.contains(Clique)){
		 Cliques.add(Clique);}
		 for(int ichi=0;ichi<Names.size();ichi++){
		 for(int aleph=0; aleph<Clique.size();aleph++){
			 if(Names.get(ichi).name.equals(Clique.get(aleph))){Names.remove(ichi);}
		 }}
		 if(Clique.isEmpty()){Names.remove(0);}
		//System.out.println(Names);
	 }
	 while(! Cliques.isEmpty()){ 
		 ArrayList<String> temp= Cliques.get(0);
		 ArrayList<Person> peopleList=new ArrayList(10);
		 for(int i=0; i<temp.size(); i++){
			 for(int j=0; j<newgraph.length;j++){
				 if(newgraph[j].nodeData.name.equals(temp.get(i))){
					 Person p= new Person(newgraph[j].nodeData.name,newgraph[j].nodeData.school,i);
					 peopleList.add(p);
				 }
			 }
		 }
		 int numPeople=temp.size(); ArrayList<String> friendList= new ArrayList(10);
		while(! temp.isEmpty()){
			 for(int z=0;z<temp.size();z++){
				 if(isFriend(temp.get(0),temp.get(z),newgraph)){
					 friendList.add(temp.get(0) + "|" + temp.get(z));
					 
				 }
				 
			 }
			 temp.remove(0);
		 }
		//Now we can perfectly recreate the graphbuilding algorithm I made, without calling it in a file;
		PNode[] sandwich= new PNode[numPeople];
		//System.out.println("Line 88");
		for(int a=0; a<peopleList.size();a++){
			Person newdude= new Person(peopleList.get(a).name,peopleList.get(a).school,a);
			PNode alpha=new PNode(null, null,newdude);
			sandwich[a]=alpha;
		}
		//System.out.println("Line 393");
		//System.out.println("Friends + People:");
		//System.out.println(friendList);
		//System.out.println(peopleList.size());
	LinkedListFill(sandwich,friendList,peopleList);
//	System.out.println("line 395");
	MirrorFriends(sandwich);
		GraphZ.add(sandwich);
		for(int i=0;i<sandwich.length;i++){
		//	System.out.println("Line 398 Debug Check");
			//System.out.println(sandwich[i].nodeData.school);
	//	System.out.println(sandwich[i].nodeData.name);
		
		
		}
		Cliques.remove(0);
		
		
		
	 }
	
		
		 
	 
	 
	 
	 return GraphZ;
 }
 
 
 
 //ALGORITHM: THE LAST

 //Connectors
 //What I do in this algorithm is once again utilize the awesome power of the shortest path algorithm.
 //Clearly if a shortest path doesn't exist between two vertices, then there is no path between them.
 //So I just pick any two vertices that aren't directly next to each other(i.e.  Person i and Person j cannot be connectors with each other, we need a K)
 //Then iterating through all possible Person k's, if we take them out and there is no shortest path(i.e. no path exists), then clearly k is a connector.
 //I think at this point you have realized that I try to be as clever so I do not have to work too hard. 
 
 public static ArrayList<String> listConnector(PNode[] graph){ ArrayList<String> connectors=new ArrayList(10);
	 for(int i=0;i<graph.length;i++){boolean isAye=false;
		 for(int j=0; j<graph.length;j++){
			 Person aye=graph[i].nodeData;
			 Person jay=graph[j].nodeData;
			 if(aye.frands.contains(jay)){isAye=false; continue;}
			
			 if(ShortestPath(aye.name,jay.name,graph).lastIndexOf("not reachable by this person")==-1){
				 for(int k=0;k<graph.length; k++){
					 PNode[] newgraph=new PNode[graph.length-1];
					 if(k!= j && k!=i){
					int alpha=0; int beta=0;	 
						 while(alpha<graph.length){
							 if(alpha!=k){newgraph[beta]=graph[alpha]; beta++; alpha++; continue;}
							 alpha++;
						 }
					
					 
					 if(ShortestPath(aye.name,jay.name,newgraph).lastIndexOf("not reachable by this person")!=-1){ 
						 if(! connectors.contains(graph[k].nodeData.name)){ connectors.add(graph[k].nodeData.name); }
					 }
				 }
			 }
		 }
	 }
	 }
	 
	 
	 return connectors;
 }
 
 
 
}
