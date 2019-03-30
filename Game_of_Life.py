def init_list(size):
	list_obj=list()
	for i in range(size):
		list_obj.append(list())
	return list_obj

def create_matrix(line, col):
        matrix=init_list(line)
        for i in range(line):
                matrix[i]=init_list(col)
        return matrix

def list_of_lines(filename):
    with open(filename) as f:
        lines=f.readlines()
        for i in range(len(lines)):
            lines[i]=lines[i].rstrip("\n")
    return lines

def file_check(filename_in,filename_out):
    chk=True
    try:
        f_in=open(filename_in)        
    except:
        chk=False
        with open(filename_out,'w') as f_out:
            f_out.write("No such input file as {0}!".format(filename_in))
    finally:
        return chk

def evolution(table):
    height=len(table)
    width=len(table[0])
    board=table
    new_board=create_matrix(height, width)
    for i in range(height):
        for j in range(width):
            neighbours_ij = 0
            for a in range(i-1,i+2):
                for b in range(j-1,j+2):
                    if board[(a + height) % height][(b + width) % width]:
                        neighbours_ij+=1
            if board[i][j]:
                neighbours_ij-=1
            if neighbours_ij==3 or (neighbours_ij==2 and board[i][j]!=0):
                new_board[i][j]=1
            else:
                new_board[i][j]=0
    return new_board

def read_file_in_normal(filename_in):
    lines=list_of_lines(filename_in)
    height=len(lines)
    width=len(lines[0])
    board=create_matrix(height,width)
    for i in range(height):
        for j in range(width):
            board[i][j]=int(lines[i][j])
    return board

def read_file_in_comprimat(filename_in):  
    with open(filename_in) as f:
        ln=list(f.readline().split(" "))
        height=int(ln[0]); width=int(ln[1])
        board=create_matrix(height,width)
        for i in range(height):
                for j in range(width):
                    board[i][j]=0
        for l in f:
            lin=list(l.split(" "))
            board[int(lin[0])][int(lin[1])]=1
    return board

def board_file_write(board, filename_out):
    with open(filename_out,'w') as f:
        height=len(board)
        width=len(board[0])
        for i in range(height):
            for j in range(width):
                f.write(str(board[i][j]))
            if i<height-1:
                f.write('\n')

def game_of_life_normal(nr_pasi,file_in, file_out, read_func):
    steps=0
    board=read_func(filename_in)
    height=len(board)
    width=len(board[0])
    if height<3 or width<3:
        with open(filename) as f_out:
            f_out.write("Invalid Configuration!")
            return -1
    while steps<=nr_pasi:
        if steps==nr_pasi:
            board_file_write(board,file_out)
        board = evolution(board)
        steps+=1

def game_of_life_stabil(nr_pasi,file_in, file_out, read_func):
    steps=0; nr_config=0
    board=read_func(filename_in)
    height=len(board)
    width=len(board[0])
    if height<3 or width<3:
        with open(file_out,'w') as f_out:
            f_out.write("Invalid Configuration!")
            return -1
    array_of_boards=list()
    pozitie_configuratie_stabila=-1
    while steps<=nr_pasi:
        if board in array_of_boards:
            pozitie_configuratie_stabila=array_of_boards.index(board)
        if pozitie_configuratie_stabila>=0:
            with open(file_out,'w') as fo:
                fo.write("Stable configuration found at step: {0}\n\n".format(pozitie_configuratie_stabila))
                board_file_write(array_of_boards[pozitie_configuratie_stabila],file_out)
                break
        elif (steps==nr_pasi) and (pozitie_configuratie_stabila==-1):
            with open(file_out,'w') as fo:
                fo.write("No Stable Configuration Found!\n\n")
                board_file_write(board,file_out) 
        array_of_boards.append(board)
        board=evolution(board)
        steps+=1

import sys
read_func=read_file_in_normal; read_option=0; game_of_life=game_of_life_normal
if '-in' in sys.argv:
    filename_in=sys.argv[sys.argv.index('-in')+1]
if '-out' in sys.argv:
    filename_out=sys.argv[sys.argv.index('-out')+1]
if '-steps' in sys.argv:
    steps_count=int(sys.argv[sys.argv.index('-steps')+1])
if '-comprimat' in sys.argv:
    read_func=read_file_in_comprimat
if '-stabil' in sys.argv:
    game_of_life=game_of_life_stabil
if file_check(filename_in,filename_out):
    game_of_life(steps_count,filename_in,filename_out,read_func)
