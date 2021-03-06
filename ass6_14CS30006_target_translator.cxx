#include "ass6_14CS30006_translator.h"

extern int yyparse();
extern symboltable GT;
extern symboltable *curr_sym;
extern type_inf *global_type;
extern Quad_Array Q_arr;
extern int global_width;
extern vector<string> string_lits;
extern symboltable *sym;





void Codegen:: set_GOTO_Labels_target()
{
    
	vector<Quad>::iterator itr = Q_arr.arr.begin();
    while(itr != Q_arr.arr.end())
    {
        if(itr->op >= IF_LESS && itr->op <= GOTO)
        {
            int backpatch_result = atoi(((*itr).result).c_str());
            if(gen_goto.count(backpatch_result)==0)
            {
                g_t_c++;
		string t = ".L";
    		t = t + to_string(g_t_c-1);
    		
    		
                gen_goto[backpatch_result] = t;
            }
            (*itr).result = gen_goto[backpatch_result];
        }
	itr++;
    }
}

void Codegen:: Map_TAC_to_Assembly()
{
    int a2_b=0,a1_b=0,  result_b=0;
    symtab *global_arg1=NULL, *global_arg2=NULL, *global_result=NULL,* c_a1,*c_a2,*c_res;    
    c_a1 = curr_sym->lookup(cur_q.arg_1);
    c_a2= curr_sym->lookup(cur_q.arg_2);
    c_res = curr_sym->lookup(cur_q.result); 

   
    for(int j = 2; j < (&GT)->no_of_entries+2; j++)
    {
        
	if((&GT)->sym_table[j].id.compare(cur_q.arg_2)==0)
            global_arg2 = &((&GT)->sym_table[j-2]);
	if((&GT)->sym_table[j].id.compare(cur_q.arg_1)==0)
            global_arg1 = &((&GT)->sym_table[j-2]);
        
    }
   
    for(int j = 2; j < (&GT)->no_of_entries+2; j++)
    {
        if((&GT)->sym_table[j-2].id.compare(cur_q.result)==0)
            	global_result =     &((&GT)->sym_table[j]);
        else 
        global_result = NULL;
    }
    
    

    string gen_a1 = emptystr;
    string gen_a2 = emptystr;
    string gen_res = emptystr;

    if(cur_q.result[0] <='0'  ||  cur_q.result[0] >='9')
    {
        if(global_result == NULL)
        {
            result_b = c_res->offset;
            string bind = to_string(result_b);
            gen_res = bind + "(%rbp)";
        }
        else
            gen_res = cur_q.result;
    }
    

    if(cur_q.arg_1[0] <= '0' || cur_q.arg_1[0] >= '9')
    {
        if(global_arg1 == NULL)
        {
            a1_b = c_a1->offset;
	    string bind = to_string(a1_b);
            gen_a1 = bind + "(%rbp)";
            
        }
        else
	{
	    if(global_arg1->type.type_name.compare("function")==0)
	    {
		func_type = 1;		
	    }
	    else
                {gen_a1 = cur_q.arg_1;}
	}

    }            

        if(cur_q.arg_2[0] <= '0' || cur_q.arg_2[0] >= '9')
        {
        if(global_arg2 == NULL)
        {
            a2_b = c_a2->offset;
	    string bind = to_string(a2_b);
            gen_a2 = bind + "(%rbp)";
        }
        else
	{
	    gen_a2 = cur_q.arg_2;
	}
            
    }            

    
    switch(cur_q.op){
    case PLUS:
    {
        

	if((cur_q.arg_2.compare("1"))==0)
        {
            cout<< "\tmovl\t" <<gen_a1<<",\t%edx\n";
            cout<< "\taddl\t$1,\t%edx\n";
            cout<< "\tmovl\t%edx,\t%eax\n";
            cout<< "\tmovl\t%eax,\t"<< gen_res<<"\n";
        }
        else
        {
            
            cout<<"\tmovl\t" <<gen_a1<<",\t%edx\n";
            if(cur_q.arg_2[0]>='0' && cur_q.arg_2[0]<='9')
                cout<<"\tmovl\t$" <<cur_q.arg_2<<",\t%eax\n";
            else
                cout << "\tmovl\t" <<gen_a2 << ",\t%eax\n";
            cout<<"\taddl\t %edx,\t%eax\n";
            cout<<"\tmovl\t %eax,\t"<<gen_res<<"\n";     
        }
    break;
    }

    case COPY:
    {
	if(func_type==1)
	{
	    
	    cout<<"\tmovl\t%eax,\t"<<gen_res<<endl;
	    func_type = 0;
	    
	}
	else
	{
		if(cur_q.arg_1[0]>='0' && cur_q.arg_1[0]<='9')
		{
		    cout<<"\tmovl\t$"<<cur_q.arg_1<<",\t"<<gen_res<<"\n";
		}

		else
		{
		    cout<<"\tmovl\t"<<gen_a1<<",\t%eax\n"; 
		    cout<<"\tmovl\t"<<"%eax,\t"<<gen_res<<"\n"; 
		}
	}
	break;
    }

    case UNARY_MINUS:
    {
	cout << "\tmovq" << "\t" <<gen_a1<< ",\t%rax" << endl; 
        cout << "\tnegq\t %rax"<< endl;
        cout << "\tmovq" << "\t%rax,\t" << gen_res << "\t"  << endl;         
	break;
    }



    case MODULO:
    {
          cout << "\tmovl\t" <<gen_a1 << ",\t%eax"<<endl;
        cout << "\tcltd\n";
        cout << "\tidivl\t"<< gen_a2 << "\n";
        cout << "\tmovl\t%edx,\t"  << gen_res << "\n";       
        break;
    }

        case MINUS:
    {
        if((cur_q.arg_2).compare("1")==0)
        {
	    //result = a + 1
            cout << "\tmovl\t" <<gen_a1<< ",\t%edx"<<endl;
            cout << "\tsubl\t$1,\t %edx"<<endl;
            cout << "\tmovl\t %edx,\t %eax"<<endl;
            cout << "\tmovl \t %eax,\t"  << gen_res<<endl;
        }
        else
        {
            

            cout<< "\tmovl\t" <<gen_a1<< ",\t%edx"<<endl;
            cout<< "\tmovl\t" <<gen_a2 << ",\t%eax"<<endl;
            cout<< "\tsubl\t%eax,\t%edx"<<endl;
            cout<< "\tmovl\t%edx,\t%eax"<<endl;
            cout<< "\tmovl \t%eax,\t" << gen_res<<endl;       
        }
	break;
    }
    

	case MULT:
    {
        
        cout<< "\tmovl\t" <<gen_a1 << ",\t%eax\n";
        if(cur_q.arg_2[0]>='0' && cur_q.arg_2[0]<='9')
            cout<< "\timull\t$" << cur_q.arg_2 << ",%eax\n";
        else
        {
            cout<< "\timull\t" <<gen_a2 << ",%eax\n";
        }
        cout<< "\tmovl\t%eax,\t"<< gen_res<< "\n";           
        break;
    }
    case DIVIDE:
    {
          

	cout << "\tmovl\t" <<gen_a1 << ",\t%eax\n";
        cout << "\tcltd\n";
        cout << "\tidivl\t"<< gen_a2 << "\n";
        cout << "\tmovl\t%eax,\t" << gen_res<< "\n";  
	break;   
        
    }

    
    case IF_LESS:
    {
        
        cout << "\tmovl\t" <<gen_a1 << ",\t%eax\n";
        cout << "\tcmpl\t" <<gen_a2 << ",\t%eax\n";
        cout << "\tjge\t.L" << g_t_c <<"\n";
        cout << "\tjmp\t" <<cur_q.result <<"\n";
        cout << ".L" << g_t_c  << ":\n";
        g_t_c = g_t_c + 1;
	break;

    }

  

    case IF_NOT_EQUAL:
    {
        
        
       

	cout << "\tmovl\t" <<gen_a1 << ",\t%eax"<<endl;
        cout << "\tcmpl\t" <<gen_a2 << ",\t%eax"<<endl;
        cout << "\tje\t.L" << g_t_c <<endl;
        cout << "\tjmp\t" <<cur_q.result <<endl;
        cout << ".L" << g_t_c  << ":\n";
        g_t_c = g_t_c + 1;
	break;
    }
     
    case IF_GREATER:
    {
        
        cout<< "\tmovl\t" <<gen_a1 << ",\t%eax"<<endl;
        cout<< "\tcmpl\t" <<gen_a2 << ",\t%eax"<<endl;
	
        cout<< "\tjle\t.L" << g_t_c <<endl;
        cout<< "\tjmp\t" <<cur_q.result <<endl;
        cout<< ".L"<< g_t_c  << ":\n";
        g_t_c = g_t_c + 1;break;

    }

    
    
    case IF_NOT_EXPRESSION:
    {
	//if a==0 goto .L1
        cout<< "\tcmpl\t$0,\t"<<gen_a1<< "\n"; 
        cout<< "\tjne\t.L"<< g_t_c<<"\n"; 
        cout<< "\tjmp\t"<<cur_q.result<<endl;
        cout<< ".L"<<g_t_c<<" : \n";  
        g_t_c = g_t_c + 1;   break;  
    }

    case IF_IS_EQUAL:
    {
        //  if a != b goto .L1

	 cout << "\tmovl\t" <<gen_a1 << ",\t%eax\n";
        cout << "\tcmpl\t" <<gen_a2 << ",\t%eax\n";
        cout << "\tjne\t.L" << g_t_c << "\n";
        cout << "\tjmp\t" <<cur_q.result << "\n";
        cout << ".L" << g_t_c  << ":\n";
        g_t_c = g_t_c + 1;break;
     }

    case IF_EXPRESSION:
    {  
	//if a!= 0 goto .L1
        cout<< "\tcmpl\t$0,\t"<<gen_a1<< endl; 
        cout<< "\tje\t.L"<< g_t_c<<endl; 
        cout<< "\tjmp\t"<<cur_q.result<<endl;
        cout<< ".L" <<g_t_c<<" : \n" ;  
        g_t_c++;break;
    }

  
    case GOTO:
    {
       
        cout<<"\tjmp\t" <<cur_q.result<<endl;break;
    }


    case PARAM:
    {
	
        if(next_q.result.compare("printi")==0 || next_q.result.compare("prints")==0 || next_q.result.compare("readi")==0)
	{
	    if(cur_q.result[0]=='.')
	    {
		cout<<"\tmovq\t$"<<cur_q.result<<",\t%rdi\n";
	    }
	    else if(cur_q.result[0]>='0' && cur_q.result[0]<='9')
                cout<<"\tmovq\t$"<<cur_q.result<<",\t%rdi\n";
            else
                cout<<"\tmovq\t"<<gen_res<<",\t%rdi\n";
            no_of_params++;
	}
	else
	{
	     string s;
             std::vector<string> str1;
	     int flag = 0;

             if(cur_q.result[0]>='0' && cur_q.result[0]<='9')
             {
                s = s + "\tmovq\t$" + cur_q.result + ",\t%rax\n";
             }
             else{
		 flag = 0;int i=0;
		 while(i<newSym->no_of_entries)
		 {
		     if(cur_q.result.compare(newSym->sym_table[i].id)==0 && newSym->sym_table[i].type.array_type.compare("array")==0)
		     {
			
		     	if(result_b>0)
				{s = s + "\tmovq\t" + gen_res + ",\t%rax\n";flag = 1;}
			else
				{s = s + "\tleaq\t" + gen_res + ",\t%rax\n";flag = 1;}
		     }
		      
		 i++;	
		 }
		 if(flag == 0)
                 	s = s + "\tmovq\t" + gen_res + ",\t%rax\n";
        	 str1.push_back(s);
        	 str1.push_back("\tpushq\t%rax\n");
        	 stack_of_parameters.push(str1);
	  
	    }
	}
        break;
    }

    case CALL:
    {
	if(cur_q.result.compare("printi")==0 || cur_q.result.compare("prints")==0 || cur_q.result.compare("readi")==0)
	{
	   int num;
           num = atoi((cur_q.arg_1).c_str());
           num=num*4;
           cout << "\t" << "call\t"<<cur_q.result << endl;
           //cout << "\t" << "addq\t$"<<num<<",\t%rsp"<< endl;
           if(cur_q.arg_2 != "")
               cout << "\t" << "movq\t%rax,\t"<<gen_a2<< endl;

	}
	else{
	int num;
        num = atoi((cur_q.arg_1).c_str());
        num=num*8;
        while(stack_of_parameters.size() > 0)
        {
            vector<string> str; 
            str = stack_of_parameters.top();
            for(int i = 0; i<str.size(); i++)
            {
                cout<<str[i];
            }
	    stack_of_parameters.pop();    //pop from stack
        }
        
        cout << "\tcall\t"<<cur_q.result << "\n";
        cout << "\taddq\t$"<<num<<",\t%rsp"<< "\n";
	cout<<"#--"<<cur_q.arg_2<<"\n";
         
	}break;

    }

    case RETURN:
    {
        if(cur_q.result.compare(emptystr)!=0)
        	cout<<"\tmovq\t"<<gen_res<< ",\t%rax"<< endl;
	cout <<"\tleave\n\tret"<<endl;
	no_of_params = 0;
	 break;
            
    }

    
    case REFERENCE:
    {
        
        cout<<"\tleaq\t"<<gen_a1<<",\t%rax\n";
        cout<<"\tmovq\t%rax,\t"<<gen_res<<"\n";break;
    }
    case DEREFERENCE:
    {
        
        cout<<"\tmovq\t"<<gen_a1<<",\t%rax\n";
        cout<<"\tmovl\t(%rax),\t %ecx\n";
        cout<<"\tmovl\t%ecx,\t"<<gen_res<<"\n";
	break;
    }



    
    case ARRAY_ACCESS:
    {
        
	if(a1_b>0)		//required for passing array as parameters
	{
		cout<<"\tmovslq\t"<<gen_a2<<",\t%rdx\n";
		cout<<"\tmovq\t"<<a1_b<<"(%rbp),\t%rdi\n";
		cout<<"\taddq\t%rdi,\t%rdx\n";
		cout<<"\tmovl\t(%rdx),\t%eax\n";
		cout<<"\tmovl\t%eax,\t"<<gen_res<<"\n";
	}
	else
	{
		cout<<"\tmovslq\t"<<gen_a2<<",\t%rdx"<< endl;
		cout<<"\tmovl\t"<<a1_b<<"(%rbp,%rdx,1),\t%eax\n";
		cout<<"\tmovl\t%eax,\t"<<gen_res<<"\n";
	} break;
    }

    case ARRAY_DEREFERENCE:
    {
        //a[i] = b
	if(result_b>0)		
	{
		cout<<"\tmovslq\t"<<gen_a2<<",\t%rdx\n";
		cout<<"\tmovl\t"<<gen_a1<<",\t%eax\n";
		cout<<"\tmovq\t"<<result_b<<"(%rbp),\t%rdi\n";
		cout<<"\taddq\t%rdi,\t%rdx\n";
		
		cout<<"\tmovl\t%eax,\t(%rdx)\n";
	}
	else{
        cout<<"\tmovslq\t"<<gen_a2<<",\t%rdx\n";
        cout<<"\tmovslq\t"<<gen_a1<<",\t%rax\n";
        cout<<"\tmovq\t%rax,\t"<<result_b<<"(%rbp,%rdx,1)\n";}break;
    }
    }


    

}


void Codegen:: h_G()
{
    symtab *p;
    int k=0;
    while(k<(&GT)->no_of_entries)
    {
        if((&GT)->sym_table[k].id[0] != 't')
        {
           
            if((&GT)->sym_table[k].type.type_name.compare("char")==0)
            {
                if((&GT)->sym_table[k].init_val == NULL)
                    cout<<"\tcomm\t"<<(&GT)->sym_table[k].id<<",1,1\n";
                else
                {
                    cout << "\t.globl\t"<<(&GT)->sym_table[k].id<<"\n";
                    cout << "\t.data"<<endl;
                    cout << "\t.type\t"<<(&GT)->sym_table[k].id<<", @object"<<"\n";
                    cout << "\t.size\t"<<(&GT)->sym_table[k].id<<", 1"<<"\n";
                    cout << (&GT)->sym_table[k].id<< ":" << "\n";
                    cout << "\t.byte\t"<<(&GT)->sym_table[k].init_val->int_val<<"\n";
                }
            }
		
	    if((&GT)->sym_table[k].type.type_name.compare("int")==0)
            {
                if((&GT)->sym_table[k].init_val == NULL)
                    cout<<"\tcomm\t"<<(&GT)->sym_table[k].id<<",4,4\n";
                else
                {
                    cout << "\t.globl\t"<<(&GT)->sym_table[k].id<<"\n";
                    cout << "\t.data"<<endl;
                    cout << "\t.align 4"<<endl;
                    cout << "\t.type\t"<<(&GT)->sym_table[k].id<<", @object"<<"\n";
                    cout << "\t.size\t"<<(&GT)->sym_table[k].id<<", 4"<<"\n";
                    cout << (&GT)->sym_table[k].id<< ":" << "\n";
                    cout << "\t.long\t"<<(&GT)->sym_table[k].init_val->int_val<<"\n";
                }
            }

        }
     k++;
    }
}


void Codegen:: Bind_mem_Record()
{
    
    //handle strings

    int g_s_count = 0;
    cout<<"\t.section\t.rodata\n";
	vector<string>::iterator itr = string_lits.begin();
    while(itr != string_lits.end())
    {
        cout<<globalstrstart<<g_s_count++<<":\n";
        cout<<"\t.string "<<*itr<<"\n";
	itr++;
    }
    
    h_G();  //handle Globals
    set_GOTO_Labels_target();

    int quad_size = Q_arr.arr.size();
	
    for(int i=0;i<quad_size;i++)
    {
	
	cout<<"  # "; Q_arr.arr[i].print();
        
        if(gen_goto.count(i)>0)
        {
            cout<<gen_goto[i]<<":\n";
        }
        cur_q = Q_arr.arr[i];	
	if(i<quad_size-1)
		next_q = Q_arr.arr[i+1];	
        
        if(Q_arr.arr[i].op == _FUNCTION_START)
        {
            if(Q_arr.arr[i+1].op != _FUNCTION_END)
            {
		ptr_type=0;
                symtab *p=NULL;
		int j=0;
                while( j < (&GT)->no_of_entries)
                {
                 
                    if(((&GT)->sym_table[j].id).compare(Q_arr.arr[i].result)==0)
                        p = &((&GT)->sym_table[j]);
		    j++;
                }
                
                func_name = Q_arr.arr[i].result;
                r = p;
		
                //newSym = curr_sym;
                //if(p!=NULL)
                newSym = p->nested_table;
                curr_sym = newSym;
                flag_param = 1;
                int total_count; 
                total_count =  r->type.no_of_params;
                //if (p==NULL)
                //    cout<<"1\n";
		for(int j = 0; j < total_count; j++)
		{
		    newSym->sym_table[j].offset = mem;
		    mem = mem + 8;
		    cout<<"#--param_offset:"<<mem<<"\n";
		}
		//cout<<"#--total:"<<total_count<<"\n";
		membindebp = 0;
                for(int j = total_count; j < newSym->no_of_entries; j++)
                {
		    if(newSym->sym_table[j].id.compare("retVal")==0)
		    {
			//newSym->sym_table[j].size = 4;
		    }
                    if(newSym->sym_table[j].id.compare("retVal"))
                    {
                        membindebp = membindebp - newSym->sym_table[j].size;
                        newSym->sym_table[j].offset = membindebp;
			cout<<"#--loacl var offset: "<<newSym->sym_table[j].size<<" "<<newSym->sym_table[j].offset<<"\n";
                    }
                }
                cout<<"#"<<membindebp<<endl;
                stack_size = membindebp*(-1) + mem;
		for(int j = 0; j< newSym->no_of_entries; j++)
		{
			if(newSym->sym_table[j].id.compare(cur_q.result)==0||!newSym->sym_table[j].id.compare(cur_q.arg_1)==0
                       ||!newSym->sym_table[j].id.compare(cur_q.arg_2)==0)
			{
				if(newSym->sym_table[j].type.pointer_type.compare("ptr")==0)
					ptr_type = 1;
				cout<<"##ptr"<<newSym->sym_table[j].id<<endl;
			}
		}
                stack_req = ((stack_size>>4)+1)<<4; 

    		cout<<"\t.globl\t"<<func_name<<"\n";	
    		cout<<"\t.type\t"<<func_name<<", @function\n";	
    		cout<<func_name<<":\n";
    		cout<<"\tpushq\t%rbp\n";
    		cout<<"\tmovq\t%rsp, %rbp\n";
    		cout<<"\tsubq\t$"<<stack_req<<",\t%rsp\n";
            }
            else
            {
                i++;
		no_of_params = 0;
		mem = 16;
		membindebp = 0;
                continue;
            }
        }
        else if(Q_arr.arr[i].op == _FUNCTION_END)
        {
            sym = &GT;
            curr_sym = &GT;
	    cout<<"\tleave\n\tret\n";
            cout << "\t.size\t"<<func_name<<",\t.-"<<func_name<<endl;
	    func_name = "";
            no_of_params = 0;
	    mem = 16;
	    membindebp = 0;
	    continue;
        }

        if(func_name.compare(emptystr))
        {
            Map_TAC_to_Assembly();
        }

    }
}


int main(int argc, char *argv[])
{   
    
    bool fail = yyparse();
    
    
    string output_file = "a.out";
    if(argc==2) output_file = string(argv[1]);
    string quad_file = "ass6_14CS30006_quads" + output_file + ".out";
    ofstream outf1(quad_file.c_str());
    streambuf *coutbuf1 = cout.rdbuf();
    cout.rdbuf(outf1.rdbuf());
int arr_s = Q_arr.arr.size();
    
    
    cout<<"      The quads are printed below\n";
    cout<<"--------------------------------------------------------\n";
  
    for(int j = 0; j<arr_s;j++)
    {
        cout<<j<<" : "; 
        Q_arr.arr[j].print();
    }

    cout<<"\n-------SYMBOL TABLE (ST.gbl)-----------"<<endl;
    symboltable *g = &GT;
    g->print();
    cout<<"----------------------------------------------------"<<endl;
   
	int i=0;
    while( i< g->no_of_entries)
    {
        if(g->sym_table[i].nested_table != NULL)
        {
            cout<<"----------------SYMBOL TABLE of "<<g->sym_table[i].id<<"----------------"<<endl;
            g->sym_table[i].nested_table->print();
            cout<<"--------------------------------------------"<<endl;
        }i++;
    }
    cout<<"----------------Compilation Result------------------\n";
    if(fail != 0)
        printf("Compile fail\n");
    else
        printf("Compile Success\n");
    cout<<"*****************************************************\n";
    
    cout.rdbuf(coutbuf1);

    string tmp = "ass6_14CS30006_" + output_file + ".s";
    ofstream outfile(tmp.c_str());
    streambuf *coutbuf = cout.rdbuf();
    cout.rdbuf(outfile.rdbuf());
    curr_sym = sym;
    Codegen c;
    c.Bind_mem_Record();
    
    cout.rdbuf(coutbuf);
    return 0;

}

