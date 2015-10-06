//insert into the hash map
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::insert(const KEY & newkey, const T & data){
	//check if need to rehash
	if (entrysize / tablesize >= 2)
		rehash();
	std::size_t key = H() (newkey);
	key = key % tablesize;
	//cout<<"hash for " << data << " in insert " << key <<endl;
	/*--------------------------------------
		          Insert element
	----------------------------------------*/
	if(find(newkey) != end() && entrysize != 0 )
		return false;
	//insert into bucket, more than one value at key location
	if (array[key] != 0){
		Entry* info = new Entry(newkey, data);
		info->next = array[key];
		array[key] = info;
		entrysize++;
		return true;
	}
	//insert into array is empty at position key
	else{
		Entry* info = new Entry(newkey, data);
		array[key] = info;
		info->next = 0;
		entrysize++;
		return true;
	}
	 return false;
}

//Erase an element in the map
template<typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::erase(const KEY& key){
		//calculate the hash value for given key
		std::size_t hash = H() (key);
		hash = hash % tablesize;
		Entry* toErase = array[hash];
		Entry* prev = toErase;//holds reference to previous node, helps deletion
		if (toErase == 0)
			return false;
		//search for key to erase
		while (toErase != 0 && toErase->key != key){
			prev = toErase;
			toErase = toErase->next;
		}
		if(toErase == 0)
			return false;
		else{
			prev->next = toErase->next;
			delete toErase;
			entrysize--;
			return true;
		}
}

//rehash the map
template <typename KEY, typename T, typename H>
void UnorderedMap<KEY, T, H>::rehash( ){
	int size = nextPrime(tablesize * 2);
	Entry** tmp; //create a hashtable 2x a big as orginal
	tmp = new Entry*[size]; 
	//set all entries to null
	for (int i = 0; i < size; i++)
		tmp[i] = 0;
	//fill hash with actual data from old table
	for (int i = 0; i < tablesize; i++){
		Entry* cur = array[i];
		while (cur != 0){
			std::size_t k = H() (cur->key);
			k = k % size;
			Entry* add = new Entry(cur->key, cur->data);
			if (tmp[k] != 0){
				add->next = tmp[k];
				tmp[k] = add;
			}
			else{
				tmp[k] = add;
				add->next = 0;
			}
			cur = cur->next;
		}
	}
	//delete current array
	for (int j = 0; j < tablesize; j++){
		Entry* cur1 = array[j];
		Entry* prev = cur1;
		while (cur1 != 0){
			cur1 = cur1->next;
			delete prev;
			prev = cur1;
		}
	}
	delete[] array;
	//delete array;
	//set array to tmp, update tablesize
	array = tmp;
	tablesize = size;
}