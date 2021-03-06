#include "cr.h"

int main(int argc, char *argv[]){
	int in, inn, val, m, n;
	int rand0, rand1;
	int inout[2];
	int input_counter = 0;
	int real_counter = 0;
	int **inputcubes;
	int **outputcubes;
	int **realcubes;
	int **inputcubes_for_process;
	int **outputcubes_for_process;
	int *variable_order;
	int *best_order;
	int best_threebit;
	int best_twobit;
	int *result;
	int best_cost = 1000000;
	int cost = best_cost + 1;
	ifstream in_stream;
	string line;


	if (argc < 3) {cout<<"File argument needed"<<endl; exit(0);}

	if (argv[2] == 0){
		in_stream.open(argv[1]);	
		getline(in_stream,line);
		while(line[0] != '.'){
			getline(in_stream,line);
		}
		while(!in_stream.eof()){
			if(line[0] == '.' && line[1] == 'i' && line[2] == ' '){
			}
			if(line[0] == '.' && line[1] == 'o' && line[2] == ' '){
			} else if(line[0] == '0' || line[0] == '1' || line[0]  == '-'){
				input_counter++;
			}
			getline(in_stream,line);
		}
	} else {
		in_stream.open(argv[1]);	
		getline(in_stream,line);
		while(line[0] != '.'){
			getline(in_stream,line);
		}
		while(!in_stream.eof()){
//			cout<<line<<endl;
			if(line[0] == '.'){
			} else {
				input_counter++;
			}
			getline(in_stream,line);
		}
	}

	inputcubes = new int*[input_counter];
	outputcubes = new int*[input_counter];
	inputcubes_for_process = new int*[input_counter];
	outputcubes_for_process = new int*[input_counter];
	result = new int[3];
	if (argv[2][0] == '0') {
		in_stream.clear();              // forget we hit the end of file
		in_stream.seekg(0, ios::beg);   // move to the start of the file
		read_pla_file(in_stream, input_counter, inout, inputcubes, outputcubes);
	} else {
		in_stream.clear();              // forget we hit the end of file
		in_stream.seekg(0, ios::beg);   // move to the start of the file
		read_real_file(in_stream, input_counter, inout, inputcubes, outputcubes);
	}
	
	in_stream.close();

	variable_order = new int[inout[0]];
	best_order = new int[inout[0]];

	for (int y = 0; y < input_counter; y++){
		inputcubes_for_process[y] = new int[inout[0]];
		outputcubes_for_process[y] = new int[inout[1]];
	}

	for (int y = 0; y < inout[0]; y++){
		variable_order[y] = y;
		best_order[y] = variable_order[y];
	}

//	cout<<"Function name: "<<argv[1]<<endl;
	//test for various input variable permutations
	for (int a = 0; a <1; a++){
//for (int a = 0; a <input_counter*inout[0]; a++){
	//for (int a = 0; a <1; a++){

		//copy the data to array used for processing the input data
		for (int y = 0; y < input_counter; y++){
			for(int o =0; o < inout[0]; o++)
				inputcubes_for_process[y][o] = inputcubes[y][o];
			for(int o =0; o < inout[1]; o++)
				outputcubes_for_process[y][o] = outputcubes[y][o];
		}

/*		cout<<" input data "<<endl;
		for(int o =0; o < inout[0]; o++){ 
			for(int u =0; u < input_counter; u++){
				if (inputcubes[u][o] == -1)
					cout<<"2, ";
				else
					cout<<inputcubes[u][o]<<", ";
			}
			cout<<endl;
		}
		for(int u =0; u < input_counter; u++)cout<<"...";
		cout<<endl;
		for(int o =0; o < inout[1]; o++){
			for(int u =0; u < input_counter; u++){
				 cout<<outputcubes[u][o]<<", ";
			}
			cout<<endl;
		}
		cout<<endl;
*/
/*		cout<<"Ordering of the variables: ";
		for (int y = 0; y < inout[0]; y++)
			cout<<variable_order[y]<<" ";
		cout<<endl;
*/
		if (argv[2][0] == '0'){
			sift_pla(0, input_counter, inout, inputcubes_for_process, outputcubes_for_process, &variable_order, result);
		} else {
			sift_real(0, input_counter, inout, inputcubes_for_process, outputcubes_for_process, &variable_order, result, true);
		}

		if (cost < best_cost){
			best_cost = result[0];
			best_threebit = result[1];
			best_twobit = result[2];
			for (int h = 0; h < inout[0]; h++)
				best_order[h] = variable_order[h];

/*			cout<<"---------- Best So Far ----------"<<endl;
			cout<<"Best Cost of the Circuit: "<<best_cost<<endl;
			cout<<"Number of Three bit gates: "<<best_threebit<<endl;
			cout<<"Number of Two bit gates: "<<best_twobit<<endl;
*/
		}
//		row_swap(input_counter, inputcubes, inout, variable_order);
	}
/*
	cout<<"---------- Best Results ----------"<<endl;
	cout<<"Function name: "<<argv[1]<<endl;
	cout<<"Best Cost of the Circuit: "<<best_cost<<endl;
	cout<<"Number of Three bit gates: "<<best_threebit<<endl;
	cout<<"Number of Two bit gates: "<<best_twobit<<endl;
*/	cout<<"Ordering of the variables: ";
	for (int y = 0; y < inout[0]; y++)
		cout<<variable_order[y]+1<<" ";
	cout<<endl;
	cout<<"Best Cost of SWAP gates: "<<result[0]<<endl;
//	cout<<"---------- Best Results ----------"<<endl;
	
return 0;
}


/*
* recursive function that reoders the PLA like input file by looking at the natural order fo the cubes
* arguments: 
*		level - level of the variable hierarchy  where to start the ordering - should be 2 
*		start - 
*
*/
void order_level_tree(int level, int start, int levelcount, int input_counter, int wires, int **inputcubes, int outputwires, int **outputcubes){
	if (level >= wires-1)
		return;
	if (levelcount < 2)
		return;
	int ocount = 0;
	int tocount = 1024;
	int icount = -1;
	int fcounter = 0;
	int icountindex;
	int cube_counter;
	int *inputOcubes[levelcount];
	int *outputOcubes[levelcount];
	int *finputOcubes[levelcount];
	int *foutputOcubes[levelcount];
	for (int y = 0; y < levelcount; y++){
		inputOcubes[y] = new int[wires];
		finputOcubes[y] = new int[wires];
		outputOcubes[y] = new int[outputwires];
		foutputOcubes[y] = new int[outputwires];
	}
	for (int a = start; a < start+levelcount; a++){
		if (tocount > inputcubes[a][level]) tocount = inputcubes[a][level];
	}
	while(ocount < levelcount){
		cube_counter = 0;
		icountindex = 0;
		for (int u = start; u < (start+levelcount); u++){
			if (tocount == inputcubes[u][level]){
				if (icount != tocount) {icount = tocount;}
				for (int v =0; v < wires;v++) inputOcubes[icountindex][v] = inputcubes[u][v];
				for (int v =0; v < outputwires;v++) outputOcubes[icountindex][v] = outputcubes[u][v];
				ocount++;
				icountindex++;
				cube_counter++;
			}
		}
		if (cube_counter > 1) {
			order_level_tree(level+2, 0, cube_counter, input_counter, wires, inputOcubes, outputwires, outputOcubes);
			for (int u = 0; u < cube_counter; u++){
				for (int v =0; v < wires;v++) 
					finputOcubes[fcounter+u][v] = inputOcubes[u][v];
				for (int v =0; v < outputwires;v++) 
					foutputOcubes[fcounter+u][v] = outputOcubes[u][v];
			}
			fcounter += cube_counter;
		} else if (cube_counter == 1){
			for (int u = 0; u < cube_counter; u++){
                                for (int v =0; v < wires;v++)
                                        finputOcubes[fcounter+u][v] = inputOcubes[u][v];
				for (int v =0; v < outputwires;v++) 
					foutputOcubes[fcounter+u][v] = outputOcubes[u][v];
			}
                        fcounter += cube_counter;
		}
		tocount++;
	}
	for (int u = 0;u < levelcount; u++){
		for (int v = 0; v < wires;v++)inputcubes[start+u][v] = finputOcubes[u][v];
		for (int v = 0; v < outputwires;v++)outputcubes[start+u][v] = foutputOcubes[u][v];
	}
	for (int y = 0; y < levelcount; y++){
		delete inputOcubes[y];
		delete finputOcubes[y];
		delete outputOcubes[y];
		delete foutputOcubes[y];
	}

}

/*
* Read the circuit specified in a .real file into array
*/
int read_real_file(std::ifstream &in_stream, int input_counter, int *inout, int **inputcubes, int **outputcubes){
	int in, out, k, icounter, vcounter, vsize, incounter;
	string line;
	char **vars;
	bool readd;
	bool found;
	char var[10];
	
//	cout<<input_counter<<" inputs detected: "<<readd<<endl;
	while(line[0] != '.'){
		getline(in_stream,line);
//		cout<< line<<"  "<<readd<<endl;
	}
	readd = false;
	icounter = 0;
	vcounter = 0;
	incounter = 0;
	while(line[0] != '.' || line[1] != 'e'){

		if(line[0] == '.' && line[1] == 'n' && line[2] == 'u'){
			string num = "";
			k = 3;
			while(line[k] != ' ') k++;
			k++;
			while(line[k] != '\0') num += line[k++];
			in = atoi(num.c_str());
//			cout <<"in: "<< in<<"  variables"<<endl;
			inout[0] = in;
			inout[1] = in;
			for (int y = 0; y < input_counter; y++){
				inputcubes[y] = new int[inout[0]];
				outputcubes[y] = new int[inout[0]];
				for (int a = 0; a < inout[0];a++){
					inputcubes[y][a] = 0;
					outputcubes[y][a] = 0;
				}
			}
/*			for (int a = 0; a < inout[0];a++){
				for (int b = 0; b <input_counter; b++){
		 			cout<<inputcubes[b][a]<<",";
				}
				cout<<endl;
			}		
			cout<<endl;

			cout<<inout[0]<<endl;
*/			vars = new char*[inout[0]];
			for (int a =0; a<inout[0]; a++){
				vars[a] = new char[20];
			}
		}

		if(line[0] == '.' && line[1] == 'v' && line[2] == 'a'){
			string num = "";
			k = 3;
//			cout<<line<<endl;
			while(line[k] != ' ')k++; 
			while(line[k] == ' ')k++; 
			icounter = 0;
			while(line[k] != '\0') {
				if(line[k] == ' ') {
					vars[vcounter][icounter] = '\0';
					icounter = 0;
					vcounter++;
				} else {
					vars[vcounter][icounter++] = line[k];
				}
//			cout<<vars[vcounter][icounter]<<endl;
				k++;
			}
			vars[vcounter][icounter] = '\0';
		}
		if (readd){
//			cout<<line<<endl;
			k = 0;
			//skip the target indication
			while(line[k] != ' ')k++;
			k++;
			vcounter = 0;
			vsize = 0;
			//while(line[k] != '\0') {
			while(true) {
				var[vcounter++] = line[k++];
				vsize++;
				if (line[k] == ' ' || line[k] == '\0'){
					var[vcounter] ='\0';
//					cout<<var<<"  ";
					for (int p = 0; p < inout[0]; p++){
						found = true;
						for(int g = 0; g < vsize; g++){
							if (var[g] != vars[p][g]) found = false;
						}
						if(found){
//							cout<<"Found match: "<<incounter<<"  :: "<<vars[p]<<endl;
							if (line[k] == '\0') {
								inputcubes[incounter][p] = 3;
							} else {
								inputcubes[incounter][p] = 1;
							}

							break;
						}
					 
					}
					vsize = 0;
					vcounter = 0;
					if (line[k] == '\0') break;
					k++;
				}
			}
			incounter++;
		}
		if(line[0] == '.' && line[1] == 'b' && line[2]  == 'e'){
			readd = true;
			icounter = 0;
		}
		getline(in_stream,line);
	}
	return 1;
}

/*
* Read the circuit specified in a .pla file into array
*/
int read_pla_file(std::ifstream &in_stream, int input_counter, int *inout, int **inputcubes, int **outputcubes){
	int in, out, k, icounter;
	string line;
/*	cout<<input_counter<<" inputs detected"<<endl;
	while(line[0] != '.'){
		getline(in_stream,line);
		cout<< line<<endl;
	}
*/	icounter = 0;
	while(line[0] != '.' || line[1] != 'e'){
		if(line[0] == '.' && line[1] == 'i' && line[2] == ' '){
			string num = "";
			k = 3;
			while(line[k] != '\0') num += line[k++];
			in = atoi(num.c_str());
//			cout <<"in: "<< in<<"  "<<line<<endl;
			inout[0] = in;
			for (int y = 0; y < input_counter; y++){
				inputcubes[y] = new int[inout[0]];
			}

		}
		if(line[0] == '.' && line[1] == 'o' && line[2] == ' '){
			string num = "";
			k = 3;
			while(line[k] != '\0') num += line[k++];
			out = atoi(num.c_str());
//			cout <<"out: "<< out<<"  "<<line<<endl;
			inout[1] = out;
			for (int y = 0; y < input_counter; y++){
				outputcubes[y] = new int[inout[1]];
			}
		} else if(line[0] == '0' || line[0] == '1' || line[0]  == '-'){
			for (int l = 0; l < inout[0]; l++){
				if(line[l] == '0' || line[l] == '1')	
					inputcubes[icounter][l] = line[l] - '0';
				else {
					inputcubes[icounter][l] = -1;
				}
			}
			for (int l = 0; l < inout[1]; l++){
                                if(line[l+inout[0]+1] == '0' || line[l+inout[0]+1] == '1')
                                        outputcubes[icounter][l] = line[l+inout[0]+1] - '0';
                        }
			icounter++;
		}
		getline(in_stream,line);
	}
	return 1;
}


/***************************************
* sifts the circuit according to the analytic method
* by calculating each cost of gate w.r.t. to target qubit - value 3
****************************************/
void sift_real(int full, int input_counter, int *inout, int **inputcubes, int **outputcubes, int **var_order, int *result, bool exact){
	int counter, tocount, ocounter, m, bestsum;
	int level, icountindex, gatecost, ancilla, twobitg, threebitg, dcarecount, gatecostmin;
	int target;
	int cube_counter;
	int tem;
	int *temp;
	int *mccs;
	int *qws;
	int *best_var_order;
	int *last_var_order;
	int *runn_var_order;
	int *reorder_array[inout[0]];
	int *inputOcubes[input_counter*4];
	int *outputOcubes[input_counter*4];
	short *wirearray[input_counter];

	best_var_order = new int[inout[0]];
	last_var_order = new int[inout[0]];
	runn_var_order = new int[inout[0]];
	for (int p = 0; p < inout[0]; p++){
		best_var_order[p] = (*var_order)[p];
		last_var_order[p] = (*var_order)[p];
		runn_var_order[p] = (*var_order)[p];
	}
	qws = new int[inout[0]];

	for (int x = 0; x < (inout[0]); x++){
		reorder_array[x] = new int[input_counter*4];
	}
	for (int y = 0; y < input_counter; y++){
		wirearray[y] = new short[inout[0]];
	}
	for (int y = 0; y < input_counter*4; y++){
		inputOcubes[y] = new int[inout[0]];
		for (int x = 0; x < inout[0]; x++){
			inputOcubes[y][x] = 0;
		}
		outputOcubes[y] = new int[inout[1]];
		for (int x = 0; x < inout[1]; x++){
			outputOcubes[y][x] = 0;
		}
	}



	for(int u =0; u < input_counter; u++){
		for(int o =0; o < inout[0]; o++) wirearray[u][o] = inputcubes[u][o];
	}

	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			reorder_array[o][u] = wirearray[u][o];
		}
	}

/*
	//function representation output
	cout<<"---------------------------"<<endl;
	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<wirearray[u][o]<<", ";
		}
		cout<<endl;
	}

	cout<<"---------------------------"<<endl;
*/
/*	//function representation output
	cout<<"Original------------------"<<endl;
	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<reorder_array[o][u]<<", ";
		}
		cout<<endl;
	}
	cout<<"---------------------------"<<endl;
*/

	//calculate the QWS once for all
//	cout<<"QWSs:\n";
	for(int o =0; o < inout[0]; o++){ 
		qws[o] = 0;
		for(int u =0; u < input_counter; u++){
			if (reorder_array[o][u]> 0) qws[o]++;
		}
//		cout<<"Qubit "<<o<<" :"<<qws[o]<<endl;
	}
/*
		for (int p = 0; p < inout[0]; p++){
			cout<<" wire/cost: "<<(*var_order)[p]<<"/"<<qws[p];
		}
		cout<<endl;
	cout<<endl;
*/

//	cout<<"MCCs:\n";
	mccs = new int[input_counter];
	int sum = 0;
	for(int u = 0; u<input_counter; u++){
		for (int o =0; o<inout[0]; o++){
			if (reorder_array[o][u] > 0){
				target = o;
			}
		}
		mccs[u] = 0;
		//For each qubit that needs to be moved reorder the array into LNN arrangment
		for(int o =0; o<inout[0]; o++){ 
			if (reorder_array[o][u]> 0) {
				//find a whole in the bits
				for (int j = o; j < target; j++){
//				cout<<" tar: "<<target<<" arr: "<<j ;
					if (reorder_array[j][u] == 0){
						mccs[u] += abs(j-o);
//						cout<<endl<<" diff: "<<(j-o)<<endl;

						temp = reorder_array[o];
						reorder_array[o] = reorder_array[j];
						reorder_array[j] = temp;
						
//						cout<<endl<<" diff: "<<(j-o)<<endl;
						tem = qws[o];
						qws[o] = qws[j];
						qws[j] = tem;
						
//						cout<<endl<<" diff: "<<(j-o)<<endl;
						tem = runn_var_order[o];
						runn_var_order[o] = runn_var_order[j];
						runn_var_order[j] = tem;
//						cout<<endl<<" diff: "<<(j-o)<<endl;
						break;
					}
				}
//				cout<<endl;
			}
		}
		sum += mccs[u];
	}
	bestsum = sum;
/*
	//function representation output
	cout<<"1--------------------------"<<endl;
	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<reorder_array[o][u]<<", ";
		}
		cout<<endl;
	}
	cout<<"---------------------------"<<endl;


	cout<<"Minterm sum :"<<sum;
	cout<<endl;

*/
	//reorder to the initial order
	for (int p = 0; p < inout[0]; p++){
		while (runn_var_order[p] != p){
			temp = reorder_array[runn_var_order[p]];
			reorder_array[runn_var_order[p]] = reorder_array[p];
			reorder_array[p] = temp;

			tem = qws[runn_var_order[p]];
			qws[runn_var_order[p]] = qws[p];
			qws[p] = tem;

			tem = runn_var_order[p];
			runn_var_order[p] = runn_var_order[tem];
			runn_var_order[tem] = tem;
		}
	}
	//function representation output
/*	cout<<"2--------------------------"<<endl;
	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<reorder_array[o][u]<<", ";
		}
		cout<<endl;
	}
	cout<<"---------------------------"<<endl;

*/



	/* Main routine */
	for(int o =0; o < inout[0]; o++){ 
		/* Reorder the qubits one at the time*/
		for(int p =o+1; p < inout[0]; p++){ 
			if (qws[o] > qws [p]){
				temp = reorder_array[p];
				reorder_array[p] = reorder_array[o];
				reorder_array[o] = temp;

				tem = qws[p];
				qws[p] = qws[o];
				qws[o] = tem;

				tem = last_var_order[p];
				last_var_order[p] = last_var_order[o];
				last_var_order[o] = tem;
			}		
		}
	//function representation output
/*	cout<<"3--------------------------"<<endl;
	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<reorder_array[o][u]<<", ";
		}
		cout<<endl;
	}
*/
		//recalculate the cost in SWAP gates
		mccs = new int[input_counter];
		sum = 0;
		for(int u = 0; u<input_counter; u++){
			for (int o =0; o<inout[0]; o++){
				if (reorder_array[o][u]> 0){
					target = o;
				}
			}
			mccs[u] = 0;

			//For each qubit that needs to be moved reorder the array into LNN arrangment
			for(int o =0; o<inout[0]; o++){ 
				if (reorder_array[o][u]> 0) {
					//find a whole in the bits
					for (int j = o; j < target; j++){
						if (reorder_array[j][u] == 0){
//				cout<<" tar: "<<o<<" arr: "<<j<<endl ;
							mccs[u] += abs(j-o);
//							cout<<endl<<" row: "<<u<<" diff: "<<(j-o)<<endl;
							
							temp = reorder_array[o];
							reorder_array[o] = reorder_array[j];
							reorder_array[j] = temp;
											

///						cout<<endl<<" diff: "<<(j-o)<<endl;
							tem = qws[o];
							qws[o] = qws[j];
							qws[j] = tem;
						
							tem = runn_var_order[o];
							runn_var_order[o] = runn_var_order[j];
							runn_var_order[j] = tem;
							break;
						}
					}
				}
			}
	//function representation output
/*	cout<<"3.5------------------------"<<endl;
	for(int m =0; m < inout[0]; m++){ 
		for(int n =0; n < input_counter; n++){
			cout<<reorder_array[m][n]<<", ";
		}
		cout<<endl;
	}
	cout<<endl;
	cout<<" Sum: "<<mccs[u]<<endl;
*/
		sum += mccs[u];
		}
		if (sum < bestsum){
			for (int o =0; o<inout[0]; o++){
				best_var_order[o] = last_var_order[o];
				bestsum = sum;
			}
		}
	//function representation output
/*	cout<<"4--------------------------"<<endl;
	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<reorder_array[o][u]<<", ";
		}
		cout<<endl;
	}

	cout<<" Sum: "<<sum<<endl;
*/
		//Reorder the qubits to the last_var_order
		for (int p = 0; p < inout[0]; p++){
			if (last_var_order[p] != runn_var_order[p]){
				for (int q = p; q < inout[0]; q++){
					if (last_var_order[q] == runn_var_order[p]){
						temp = reorder_array[runn_var_order[q]];
						reorder_array[runn_var_order[q]] = reorder_array[p];
						reorder_array[p] = temp;
						
						tem = qws[runn_var_order[q]];
						qws[runn_var_order[q]] = qws[p];
						qws[p] = tem;
						
						tem = runn_var_order[q];
						runn_var_order[p] = runn_var_order[q];
						runn_var_order[q] = tem;
					}
				}
			}
		}
	}

	for (int p = 0; p < inout[0]; p++){
		while (last_var_order[p] != p){
			temp = reorder_array[last_var_order[p]];
			reorder_array[last_var_order[p]] = reorder_array[p];
			reorder_array[p] = temp;

			tem = last_var_order[p];
			last_var_order[p] = last_var_order[tem];
			last_var_order[tem] = tem;
		}
	}

	for (int p = 0; p < inout[0]; p++){
		(*var_order)[p] = best_var_order[p];
	}

	for(int o =0; o < inout[0]; o++){ 
		if (best_var_order[o] != o){
			for(int u =0; u < input_counter; u++){
				wirearray[u][o] = reorder_array[best_var_order[o]][u];
			}
		}
	}

	//function representation output
/*	cout<<"Best----------------------"<<endl;
	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<wirearray[u][o]<<", ";
		}
		cout<<endl;
	}
	cout<<"---------------------------"<<endl;
*/
/*	//function representation output
	cout<<"R--------------------------"<<endl;
	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<reorder_array[o][u]<<", ";
		}
		cout<<endl;
	}
*/
/*	cout<<"---------------------------"<<endl;

	cout<<" Best Sum: "<<bestsum<<endl;
	cout<<" Best order: ";
	for (int i = 0;i <inout[0];i++)
		cout<<best_var_order[i]<<", ";
	cout<<endl;
	cout<<"---------------------------"<<endl;
*/
//	exit(0);
	result[0] = bestsum;
}
/***************************************
* parameter full 
* full = 0 the pla is a completely specified Multi-input multi-output reversible function
* full = 1 the pla is a single bit - directly realizable reversible function 
* full = 2 dont't cares on the inputs and outputs
* full = 3  multi-input and single output function that requires embedding
****************************************/
void sift_pla(int full, int input_counter, int *inout, int **inputcubes, int **outputcubes, int **var_order, int *result){
	int counter, tocount, ocounter, m;
	int level, icountindex, gatecost, ancilla, twobitg, threebitg, dcarecount, gatecostmin;
	int target;
	int bestsum;
	int cube_counter;
	int *mccs;
	int *qws;
	int *reorder_array[inout[0]];
	int *inputOcubes[input_counter*4];
	int *outputOcubes[input_counter*4];
	short *wirearray[input_counter*4];

	for (int x = 0; x < (inout[0]); x++){
		reorder_array[x] = new int[input_counter*4];
	}
	for (int y = 0; y < input_counter*4; y++){
		wirearray[y] = new short[inout[0]];
		inputOcubes[y] = new int[inout[0]];
		for (int x = 0; x < inout[0]; x++){
			inputOcubes[y][x] = 0;
		}
		outputOcubes[y] = new int[inout[1]];
		for (int x = 0; x < inout[1]; x++){
			outputOcubes[y][x] = 0;
		}
	}



	/* do a two bit encoding */
	counter = 0;
	while(true){
		for(m = 0; m < input_counter; m++){
			if (inputcubes[m][counter] == 0 && inputcubes[m][counter+1] == 0){
				inputcubes[m][counter] = 2;	
				inputcubes[m][counter+1] = 2;	
			} else if (inputcubes[m][counter] == 0 && inputcubes[m][counter+1] == 1){
				inputcubes[m][counter] = 3;	
				inputcubes[m][counter+1] = 3;	
			} else if (inputcubes[m][counter] == 1 && inputcubes[m][counter+1] == 0){
				inputcubes[m][counter] = 4;	
				inputcubes[m][counter+1] = 4;	
			} else if (inputcubes[m][counter] == 1 && inputcubes[m][counter+1] == 1){
				inputcubes[m][counter] = 5;	
				inputcubes[m][counter+1] = 5;	
			} else if (inputcubes[m][counter] == 0 && inputcubes[m][counter+1] == -1){
				inputcubes[m][counter] = 6;
				inputcubes[m][counter+1] = 6;	
			} else if (inputcubes[m][counter] == 1 && inputcubes[m][counter+1] == -1){
				inputcubes[m][counter] = 7;	
				inputcubes[m][counter+1] = 7;	
                       } else if (inputcubes[m][counter] == -1 && inputcubes[m][counter+1] == 0){
				inputcubes[m][counter] = 8;	
				inputcubes[m][counter+1] = 8;	
                       } else if (inputcubes[m][counter] == -1 && inputcubes[m][counter+1] == 1){
				inputcubes[m][counter] = 9;	
				inputcubes[m][counter+1] = 9;	
                       } else if (inputcubes[m][counter] == -1 && inputcubes[m][counter+1] == -1){
				inputcubes[m][counter] = 10;	
				inputcubes[m][counter+1] = 10;	
                       }
		}
		if (counter < inout[0] - 2)
			counter += 2;
		else
			break;
	}

	for(int u =0; u < input_counter; u++){
		for(int o =0; o < inout[0]; o++) wirearray[u][o] = -1;
	}



	/* order the circuit by descending order on the input lines */
	counter = -1;	
	ocounter = 0;
	tocount = -1;
	level = 0;
	icountindex = 0;
	while(ocounter < input_counter){
		cube_counter = 0;
		for (int u = 0; u < input_counter; u++){
			if (tocount ==inputcubes[u][level]){
				if (counter != tocount) {counter = tocount;}
				for (int v =0; v < inout[0];v++) inputOcubes[ocounter][v] = inputcubes[u][v];
				if (level == 0)
					for (int v =0; v < inout[1];v++) outputOcubes[ocounter][v] = outputcubes[u][v];
				ocounter++;
				cube_counter++;
			}
		}
		order_level_tree(level+2, icountindex, cube_counter, input_counter, inout[0], inputOcubes, inout[1], outputOcubes);
		icountindex += cube_counter;
		tocount++;

	}



	/* change encoding back to positive and negative control points */
	counter = 0;
	while(true){
		for(int m = 0; m < input_counter; m++){
			if (inputOcubes[m][counter] == 2){
				wirearray[m][counter] = 2;	
				wirearray[m][counter+1] = 2;	
			} else if (inputOcubes[m][counter] == 3){
				wirearray[m][counter] = 2;	
				wirearray[m][counter+1] = 1;	
			} else if (inputOcubes[m][counter] == 4){
				wirearray[m][counter] = 1;	
				wirearray[m][counter+1] = 2;	
			} else if (inputOcubes[m][counter] == 5){
				wirearray[m][counter] = 1;	
				wirearray[m][counter+1] = 1;	
			} else if (inputOcubes[m][counter] == 6){
				wirearray[m][counter] = 2;
			} else if (inputOcubes[m][counter] == 7){
				wirearray[m][counter] = 1;	
                       } else if (inputOcubes[m][counter] == 8){
				wirearray[m][counter+1] = 2;	
                       } else if (inputOcubes[m][counter] == 9){
				wirearray[m][counter+1] = 1;	
                       }
		}
		if (counter < inout[0] - 2)
			counter += 2;
		else
			break;
	}
	for(int m = 0; m < input_counter; m++)
		if (inputOcubes[m][inout[0]-1] == 1)
			 wirearray[m][inout[0]-1] = 1;
		else if (inputOcubes[m][inout[0]-1] ==0)
			wirearray[m][inout[0]-1] = 2;

	cout<<endl;
	//function representation output
	for(int o =0; o < inout[0]*2; o++){ 
		for(int u =0; u < input_counter; u++){
			cout<<wirearray[u][o]<<", ";
		}
		cout<<endl;
	}
	cout<<endl;

	for(int o =0; o < inout[0]; o++){ 
		for(int u =0; u < input_counter; u++){
			reorder_array[o][u] = wirearray[u][o];
		}
	}

	cout<<"MCCs:\n";
	mccs = new int[input_counter];
	int sum = 0;
	for(int u = 0; u<input_counter; u++){
		target = inout[0];
		mccs[u] = 0;
		for(int o =inout[0]-1; o>=0; o--){ 
			if (reorder_array[o][u]> 0) {
				mccs[u] += target-o-1;
				target--;
			}
		}
		sum += mccs[u];
	}
	bestsum = sum;	
	cout<<"Minterm sum :"<<sum;
	cout<<endl;
	cout<<"QWSs:\n";
	qws = new int[inout[0]];
	for(int o =0; o < inout[0]; o++){ 
		qws[o] = 0;
		for(int u =0; u < input_counter; u++){
			if (reorder_array[o][u]> 0) qws[o]++;
		}
//		cout<<"Qubit "<<o<<" :"<<qws[o]<<endl;
	}

	/* Reorder the qubits */
	int *temp;
	int tem;
/*
		cout<<" address: "<<var_order<<"  "<<*var_order<<endl<<endl;;

		for (int p = 0; p < inout[0]; p++){
			cout<<" wire/cost: "<<(*var_order)[p]<<"/"<<qws[p];
		}

		cout<<endl;
	cout<<endl;
*/
	for(int o =0; o < inout[0]; o++){ 
		for(int p =o+1; p < inout[0]; p++){ 
//		cout<<o<<" * "<<p<<" : "<<" : "<<qws[o]<<" : "<<qws[p]<<" : "<<var_order<<"   "<<*var_order<<endl;
//		cout<<var_order[o]<<" "<<*var_order[o]<<",  "<<var_order[p]<<" "<<*var_order[p]<<endl;
			if (qws[o] > qws [p]){
				temp = reorder_array[p];
				reorder_array[p] = reorder_array[o];
				reorder_array[o] = temp;
				tem = qws[p];
				qws[p] = qws[o];
				qws[o] = tem;

				tem = (*var_order)[p];
				(*var_order)[p] = (*var_order)[o];
				(*var_order)[o] = tem;
			}		
		}


//	cout<<"MCCs:\n";
	mccs = new int[input_counter];
	sum = 0;
	for(int u = 0; u<input_counter; u++){
		target = inout[0];
		mccs[u] = 0;
		for(int o =inout[0]-1; o>=0; o--){ 
			if (reorder_array[o][u]> 0) {
				mccs[u] += target-o-1;
				target--;
			}
		}
		sum += mccs[u];
	}
//	cout<<"Minterm sum :"<<sum;
//	cout<<endl;

	if (bestsum > sum){
		bestsum = sum;
	}
	}
//	cout<<"MCCs:\n";
	mccs = new int[input_counter];
	sum = 0;
	for(int u = 0; u<input_counter; u++){
		target = inout[0];
		mccs[u] = 0;
		for(int o =inout[0]-1; o>=0; o--){ 
			if (reorder_array[o][u]> 0) {
				mccs[u] += target-o-1;
				target--;
			}
		}
		sum += mccs[u];
	}
//	cout<<"Minterm sum :"<<sum;
//	cout<<endl;

//		for (int p = 0; p < inout[0]; p++){
//			cout<<" wire/cost: "<<(*var_order)[p]<<"/"<<qws[p];
//		}
//
//		cout<<endl;
//	cout<<endl;

	if (bestsum > sum){
		bestsum = sum;
	}

	result[0] = bestsum;
}


