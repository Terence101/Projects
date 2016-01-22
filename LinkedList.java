public class LinkedList {
	Node head;

	public LinkedList(int val){
		head = new Node(val);
	}

	public void insert(int val){
		Node curr=head;
		Node temp = new Node(val);
		if(curr.data > val){
			temp.next = head;
			head.prev = temp;
			head = temp;
			return;
		}

		while(curr!=null){
			if(curr.data > val){
				temp.next = curr;
				curr.prev = temp;
				curr.prev.next = temp;
				temp.prev = curr.prev;
				return;
			}
			if(curr.next == null){
				curr.next = temp;
				temp.prev = curr;
				return;
			}
			curr = curr.next;
		}
	}

	public void delete(int val){
		Node curr;
		if(val==head.data){
			curr = head;
			head = head.next;
			head.prev = null;
			curr.next = null;
			return;
		}
		curr = head;
		while(curr != null){
			if(curr.data==val){
				if(curr.next==null){
					curr.prev.next = null;
					curr.prev = null;
				}else{
					curr.prev.next = curr.next;
					curr.next.prev = curr.prev;
				}
				return;
			}
			curr = curr.next;
		}
		System.out.println("item doesn't exist in the list");
	}

}

class Node{
	Node next;
	Node prev;
	int data;

	public Node(int val){
		data = val;
		next = null;
		prev = null;
	}
}
