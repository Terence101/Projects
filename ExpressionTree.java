import java.util.*;

public class ExpressionTree{

     public static void main(String []args){
        Scanner sc = new Scanner (System.in);
        System.out.println("Please enter string to be processed");
        
       String info = sc.next();
       Node tree = buildTree(info);
     }
     
     public Node buildTree(String info){
          Stack<Node> keep = new Stack<Node>();
       
       for(char x:info.toCharArray()){
           if(x!= ')'){
               Node temp = new Node(x);
               keep.push(temp);
           }else{
               Node temp2 = keep.pop();
               
               while(temp2.val != '('){
                   Node left = temp2;
                   Node operator = keep.pop();
                   Node right = keep.pop();
                   
                   operator.left = left;
                   operator.right = right;
                   
                   temp2 = keep.pop();
                   keep.push(operator);
               }
           }
       }
       
       Node temp3 = keep.pop();
       
       while(!keep.empty()){
           Node left = temp3;
           Node operator = keep.pop();
           Node right = keep.pop();
           
           operator.left = left;
           operator.right = right;
           
           keep.push(operator);
           temp3 = keep.pop();
       }
       return temp3;
    }
     
}


class Node{
    char val;
    Node left;
    Node right;
    
    public Node(char data){
        val = data;
        left = null;
        right = null;
    }
}
