//START OF PROGRAM
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#define pb push_back
#define lp(i,n) for(int i=0;i<n;i++)
#define read_int(i) scanf("%d", &i)
#define pii pair<int,int>
#define triplet pair<pair<int,int> ,char>
#define mp make_pair
#define ff first
#define ss second

using namespace std;

//prints ASCII art logo
void print_logo()
{
    char logo[7][100]={
        "      .oooooo.   ooooo   ooooo oooooooooooo  .oooooo..o  .oooooo..o ",
        "     d8P'  `Y8b  `888'   `888' `888'     `8 d8P'    `Y8 d8P'    `Y8 ",
        "    888           888     888   888         Y88bo.      Y88bo.      ",
        "    888           888ooooo888   888oooo8     `\"Y8888o.   `\"Y8888o.  ",
        "    888           888     888   888    \"         `\"Y88b      `\"Y88b ",
        "    `88b    ooo   888     888   888       o oo     .d8P oo     .d8P ",
        "     `Y8bood8P'  o888o   o888o o888ooooood8 8\"\"88888P'  8\"\"88888P'  "};

    for(int i=0;i<7;i++)
        puts(logo[i]);

}

//every square on the board is actually a piece...so the whole chess board can be considered as an array of pieces
struct piece{
    char type=0; //0-0-0-0 means empty place
    char color=0;
    bool moved=0;
};

//GLOBALS
piece board[8][8];
int attacked_by_w[8][8];
int attacked_by_b[8][8];
char boardcolor[10][10];
pair <int, int> w_king_pos;
pair <int, int> b_king_pos;

//temps
piece temp_board[8][8];
int temp_attacked_by_w[8][8];
int temp_attacked_by_b[8][8];
pair <int, int> temp_w_king_pos;
pair <int, int> temp_b_king_pos;


piece temp_board2[8][8];
int temp_attacked_by_w2[8][8];
int temp_attacked_by_b2[8][8];
pair <int, int> temp_w_king_pos2;
pair <int, int> temp_b_king_pos2;


//graphics
SDL_Window* window=NULL;
SDL_Surface* mainsurface=NULL;
SDL_Surface* guiboard=NULL;
const int SCREEN_WIDTH = 672;
const int SCREEN_HEIGHT = 694;

SDL_Surface* rook_sprite_w=IMG_Load("Chess Piece Icons/rookw.png");
SDL_Surface* bishop_sprite_w=IMG_Load("Chess Piece Icons/bishopw.png");
SDL_Surface* knight_sprite_w=IMG_Load("Chess Piece Icons/knightw.png");
SDL_Surface* queen_sprite_w=IMG_Load("Chess Piece Icons/queenw.png");
SDL_Surface* king_sprite_w=IMG_Load("Chess Piece Icons/kingw.png");
SDL_Surface* pawn_sprite_w=IMG_Load("Chess Piece Icons/pawnw.png");

SDL_Surface* rook_sprite_b=IMG_Load("Chess Piece Icons/rookb.png");
SDL_Surface* bishop_sprite_b=IMG_Load("Chess Piece Icons/bishopb.png");
SDL_Surface* knight_sprite_b=IMG_Load("Chess Piece Icons/knightb.png");
SDL_Surface* queen_sprite_b=IMG_Load("Chess Piece Icons/queenb.png");
SDL_Surface* king_sprite_b=IMG_Load("Chess Piece Icons/kingb.png");
SDL_Surface* pawn_sprite_b=IMG_Load("Chess Piece Icons/pawnb.png");

SDL_Surface* tile_w=IMG_Load("Chess Piece Icons/tilew.png");
SDL_Surface* tile_b=IMG_Load("Chess Piece Icons/tileb.png");

SDL_Surface* icon=IMG_Load("Chess Piece Icons/chessico.png");

SDL_Window* continue_window=NULL;
SDL_Surface* continue_window_surface=NULL;
SDL_Surface* continue_window_image=NULL;
//END GLOBALS


//this function creates the main window and draws the chess board on it
void init_gui()
{
    window = SDL_CreateWindow("CHESS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_SetWindowIcon(window,icon);
    mainsurface = SDL_GetWindowSurface(window);

    guiboard = SDL_LoadBMP("Chess Piece Icons/Chess board.bmp");

    SDL_BlitSurface(guiboard,NULL,mainsurface,NULL);

    SDL_UpdateWindowSurface(window);
}

//the same as SDL_BlitSurface function but modified in a way that allows entering the destination coordinates of drawing
void custom_blit(int x, int y, SDL_Surface* from, SDL_Surface* to)
{
    SDL_Rect temp;

    temp.x=x;
    temp.y=y;

    SDL_BlitSurface(from,NULL,to,&temp);
}

//changes board[][] index to pixel coordinates (y-axis)
int row_to_pixel(int y)
{
    return 76 + y*67;
}

//changes board[][] index to pixel coordinates (x-axis)
int col_to_pixel(int x)
{
    return 71 + x*67;
}

//changes pixel coordinates (y-axis) to array index
int pixel_to_row(int px)
{
    if(px<76) return -1;
    int r= (px-76)/67;

    if(r>=8) r=-1;
    return r;

}

//changes pixel coordinates (x-axis) to array index
int pixel_to_col(int px)
{
     if(px<71) return -1;
    int r= (px-71)/67;
    if(r>=8) r=-1;
    return r;

}

//initializes the GUI board with every piece in its correct place as in the start of any chess game
void reset_board_gui()
{
    for(int i=0;i<8;i++)
        custom_blit(col_to_pixel(i),row_to_pixel(1),pawn_sprite_b,mainsurface);

    for(int i=0;i<8;i++)
        custom_blit(col_to_pixel(i),row_to_pixel(6),pawn_sprite_w,mainsurface);

    custom_blit(col_to_pixel(0),row_to_pixel(0),rook_sprite_b,mainsurface);
    custom_blit(col_to_pixel(7),row_to_pixel(0),rook_sprite_b,mainsurface);
    custom_blit(col_to_pixel(0),row_to_pixel(7),rook_sprite_w,mainsurface);
    custom_blit(col_to_pixel(7),row_to_pixel(7),rook_sprite_w,mainsurface);

    custom_blit(col_to_pixel(1),row_to_pixel(0),knight_sprite_b,mainsurface);
    custom_blit(col_to_pixel(6),row_to_pixel(0),knight_sprite_b,mainsurface);
    custom_blit(col_to_pixel(1),row_to_pixel(7),knight_sprite_w,mainsurface);
    custom_blit(col_to_pixel(6),row_to_pixel(7),knight_sprite_w,mainsurface);

    custom_blit(col_to_pixel(2),row_to_pixel(0),bishop_sprite_b,mainsurface);
    custom_blit(col_to_pixel(5),row_to_pixel(0),bishop_sprite_b,mainsurface);
    custom_blit(col_to_pixel(2),row_to_pixel(7),bishop_sprite_w,mainsurface);
    custom_blit(col_to_pixel(5),row_to_pixel(7),bishop_sprite_w,mainsurface);

    custom_blit(col_to_pixel(3),row_to_pixel(0),queen_sprite_b,mainsurface);
    custom_blit(col_to_pixel(3),row_to_pixel(7),queen_sprite_w,mainsurface);

    custom_blit(col_to_pixel(4),row_to_pixel(0),king_sprite_b,mainsurface);
    custom_blit(col_to_pixel(4),row_to_pixel(7),king_sprite_w,mainsurface);

    SDL_UpdateWindowSurface(window);
}

int castling_w(pii to);
int castling_b(pii to);

//moves a piece of any type from one square to another whether by eating an enemy piece or even if destination square is empty
void move_piece_gui(pii a, pii b)
{
        pii x=a, y=b;

// CASTLING
            if (x==w_king_pos){
                if(castling_w(y)==1) {

                    custom_blit(col_to_pixel(x.second),row_to_pixel(x.first), boardcolor[x.second][x.first]=='w'?tile_w:tile_b,mainsurface);
                    custom_blit(col_to_pixel(y.second+1),row_to_pixel(y.first), boardcolor[y.first][y.second+1]=='w'?tile_w:tile_b,mainsurface);
                    custom_blit(col_to_pixel(6),row_to_pixel(7), king_sprite_w,mainsurface);
                    custom_blit(col_to_pixel(5),row_to_pixel(7), rook_sprite_w,mainsurface);
                    return;
                }

                if(castling_w(y)==-1) {

                    custom_blit(col_to_pixel(x.second),row_to_pixel(x.first), boardcolor[x.first][x.second]=='w'?tile_w:tile_b,mainsurface);
                    custom_blit(col_to_pixel(y.second-2),row_to_pixel(y.first), boardcolor[y.first][y.second-2]=='w'?tile_w:tile_b,mainsurface);
                    custom_blit(col_to_pixel(3),row_to_pixel(7), king_sprite_w,mainsurface);
                    custom_blit(col_to_pixel(4),row_to_pixel(7), rook_sprite_w,mainsurface);
                    return;
                }
            }



            if (x==b_king_pos){
                if(castling_b(y)==1) {
                    custom_blit(col_to_pixel(x.second),row_to_pixel(x.first), boardcolor[x.first][x.second]=='w'?tile_w:tile_b,mainsurface);
                    custom_blit(col_to_pixel(y.second+1),row_to_pixel(y.first), boardcolor[y.first][y.second+1]=='w'?tile_w:tile_b,mainsurface);
                    custom_blit(col_to_pixel(6),row_to_pixel(0), king_sprite_b,mainsurface);
                    custom_blit(col_to_pixel(5),row_to_pixel(0), rook_sprite_b,mainsurface);
                    return;
                }

                if(castling_b(y)==-1) {
                    custom_blit(col_to_pixel(x.second),row_to_pixel(x.first), boardcolor[x.first][x.second]=='w'?tile_w:tile_b,mainsurface);
                    custom_blit(col_to_pixel(y.second-2),row_to_pixel(y.first), boardcolor[y.first][y.second-2]=='w'?tile_w:tile_b,mainsurface);
                    custom_blit(col_to_pixel(0),row_to_pixel(3), king_sprite_b,mainsurface);
                    custom_blit(col_to_pixel(0),row_to_pixel(4), rook_sprite_b,mainsurface);
                    return;

                }
            }

//END CASTLING


        if(board[a.first][a.second].type=='P'){
            if(boardcolor[b.second][b.first]=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_b,mainsurface);

            if(board[a.first][a.second].color=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),pawn_sprite_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),pawn_sprite_b,mainsurface);

            if(boardcolor[a.second][a.first]=='w')
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_b,mainsurface);
        }
        /////////////////////////
        else if(board[a.first][a.second].type=='Q'){
            if(boardcolor[b.second][b.first]=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_b,mainsurface);

            if(board[a.first][a.second].color=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),queen_sprite_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),queen_sprite_b,mainsurface);

            if(boardcolor[a.first][a.second]=='w')
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_b,mainsurface);
        }
        /////////////////////////
        else if(board[a.first][a.second].type=='R'){
            if(boardcolor[b.second][b.first]=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_b,mainsurface);

            if(board[a.first][a.second].color=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),rook_sprite_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),rook_sprite_b,mainsurface);

            if(boardcolor[a.first][a.second]=='w')
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_b,mainsurface);
        }
        /////////////////////////
        else if(board[a.first][a.second].type=='N'){
            if(boardcolor[b.second][b.first]=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_b,mainsurface);

            if(board[a.first][a.second].color=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),knight_sprite_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),knight_sprite_b,mainsurface);

            if(boardcolor[a.first][a.second]=='w')
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_b,mainsurface);
        }
        /////////////////////////
        else if(board[a.first][a.second].type=='B'){
            if(boardcolor[b.second][b.first]=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_b,mainsurface);

            if(board[a.first][a.second].color=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),bishop_sprite_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),bishop_sprite_b,mainsurface);

            if(boardcolor[a.first][a.second]=='w')
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_b,mainsurface);
        }
        /////////////////////////
        else if(board[a.first][a.second].type=='K'){
            if(boardcolor[b.second][b.first]=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),tile_b,mainsurface);

            if(board[a.first][a.second].color=='w')
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),king_sprite_w,mainsurface);
            else
                custom_blit(col_to_pixel(b.second),row_to_pixel(b.first),king_sprite_b,mainsurface);

            if(boardcolor[a.first][a.second]=='w')
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_w,mainsurface);
            else
                custom_blit(col_to_pixel(a.second),row_to_pixel(a.first),tile_b,mainsurface);
        }
        /////////////////////////
}//p q r n b k

//a function that adjusts the type,color and moved attributes to any piece given to it
piece declare_piece(char type, int color, bool moved=false){
    piece p;
    p.type=type;
    p.color=color;
    p.moved=moved;
    return p;

}

//initializes the board with every piece in its correct place as in the start of any chess game BUT WITHOUT GUI
void reset_board(){

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){

            board[i][j]=declare_piece(0,0,0);

        }

    }


    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if((i+j)%2==0)
                boardcolor[i][j]='w';
            else
                boardcolor[i][j]='b';


        }

    }

    //pawn init
    for(int i=0;i<8;i++){
        board[1][i]=declare_piece('P','b',0);
        board[6][i]=declare_piece('P','w',0);
    }

    //rook init
    board[0][7]=declare_piece('R','b',0);
    board[0][0]=declare_piece('R','b',0);

    board[7][7]=declare_piece('R','w',0);
    board[7][0]=declare_piece('R','w',0);

    //kNight init
    board[0][6]=declare_piece('N','b',0);
    board[0][1]=declare_piece('N','b',0);

    board[7][6]=declare_piece('N','w',0);
    board[7][1]=declare_piece('N','w',0);

    //bishop init
    board[0][5]=declare_piece('B','b',0);
    board[0][2]=declare_piece('B','b',0);

    board[7][5]=declare_piece('B','w',0);
    board[7][2]=declare_piece('B','w',0);

    //queen init
    board[0][3]=declare_piece('Q','b',0);
    board[7][3]=declare_piece('Q','w',0);

    //king init
    board[0][4]=declare_piece('K','b',0);
    b_king_pos=mp(0,4);
    board[7][4]=declare_piece('K','w',0);
     w_king_pos=mp(7,4);

}

//next 3 functions adjust the FUN mode
char helper_funmode(int n)
{
    char randchar;

    switch(n){
        case 0: randchar='R';break;
        case 1: randchar='B';break;
        case 2: randchar='N';break;
        case 3: randchar='Q';break;
        case 4: randchar='P';break;
    }

    return randchar;
}

void reset_board_funmode()
{
    srand(time(NULL));

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){

            board[i][j]=declare_piece(0,0,0);

        }

    }


    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if((i+j)%2==0)
                boardcolor[i][j]='w';
            else
                boardcolor[i][j]='b';


        }
    }

    //king init
    board[0][4]=declare_piece('K','b',0);
    b_king_pos=mp(0,4);
    board[7][4]=declare_piece('K','w',0);
     w_king_pos=mp(7,4);

     //black rand
     for(int i=0;i<=1;i++)
        for(int j=0;j<8;j++){

            if(board[i][j].type=='K')
                continue;

            board[i][j]=declare_piece(helper_funmode(rand()%5),'b',0);
        }

     //white rand
     for(int i=6;i<=7;i++)
        for(int j=0;j<8;j++){

            if(board[i][j].type=='K')
                continue;

            board[i][j]=declare_piece(helper_funmode(rand()%5),'w',0);
        }

}

void reset_board_gui_funmode()
{
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++){

            if(board[i][j].color=='b'){

                switch(board[i][j].type){

                    case 'R': custom_blit(col_to_pixel(j),row_to_pixel(i),rook_sprite_b,mainsurface);break;
                    case 'B': custom_blit(col_to_pixel(j),row_to_pixel(i),bishop_sprite_b,mainsurface);break;
                    case 'N': custom_blit(col_to_pixel(j),row_to_pixel(i),knight_sprite_b,mainsurface);break;
                    case 'Q': custom_blit(col_to_pixel(j),row_to_pixel(i),queen_sprite_b,mainsurface);break;
                    case 'K': custom_blit(col_to_pixel(j),row_to_pixel(i),king_sprite_b,mainsurface);break;
                    case 'P': custom_blit(col_to_pixel(j),row_to_pixel(i),pawn_sprite_b,mainsurface);break;
                }

            }else

            if(board[i][j].color=='w'){

                switch(board[i][j].type){
                    case 'R': custom_blit(col_to_pixel(j),row_to_pixel(i),rook_sprite_w,mainsurface);break;
                    case 'B': custom_blit(col_to_pixel(j),row_to_pixel(i),bishop_sprite_w,mainsurface);break;
                    case 'N': custom_blit(col_to_pixel(j),row_to_pixel(i),knight_sprite_w,mainsurface);break;
                    case 'Q': custom_blit(col_to_pixel(j),row_to_pixel(i),queen_sprite_w,mainsurface);break;
                    case 'K': custom_blit(col_to_pixel(j),row_to_pixel(i),king_sprite_w,mainsurface);break;
                    case 'P': custom_blit(col_to_pixel(j),row_to_pixel(i),pawn_sprite_w,mainsurface);break;
                }

            }

        }//end 2nd for

    SDL_UpdateWindowSurface(window);

}//end 3 FUN functions

//the next 3 functions print the chess board with its pieces in the console (DISCONTINUED)
void print_dashed_line(int n,char c='-'){
    for(int i=0; i<n; i++){ //8 types,8 colors , 8 spaces before piece, 8 spaces after , 9 bars
        cout<<c;
    }

}

void print_spacer(){
    cout<<"|   |";
    for(int i=0; i<8; i++){ //spacers (optional)
        cout<<"|    ";
    }
    cout<<"|";

}


void print_board(piece grid[8][8]){
    print_dashed_line(8+8+8+8+9+5);
    cout<<endl;

    vector <char> letters ={'a', 'b' ,'c' ,'d','e','f','g','h'};

    cout<<"|    ";

    for(int j=0; j<8; j++){
        cout<<"| "<<letters[j]<<"  ";
    }

    cout<<"|"<<endl;
    print_dashed_line(8+8+8+8+9+5,'=');
    cout<<endl;


    for(int i=0; i<8; i++){


        cout<<"| "<<8-i<<" |";
        for(int j=0; j<8; j++){
            cout<<"| "<<grid[i][j].type<<grid[i][j].color<<" ";
        }

        cout<<"|"<<endl;


        print_spacer();
        cout<<endl;

        print_dashed_line(8+8+8+8+9+5);
        cout<<endl;


    }



}

//checks if any given array indices are out of bounds or not
bool in_board(int i, int j){
    if(i>7 || j>7 || i<0 || j<0) return false;
    return true;

}


//Converts from Chess Board Notation to Array index in board[8][8] return -1,-1 if invalid (DISCONTINUED)
pair<int,int> notation_to_index(string input)
{
    pair<int,int>converted(-1,-1);

    if(input.size()>2)
        return converted;

    if(input[1]>='1' && input[1]<='8')
        converted.first = 8 - (input[1] - '0');     //row index
    else
        return converted;							//invalid


    if(input[0]>='a' && input[0]<='h')
        converted.second = input[0]-97;			    //column index if lowercase letter

    else
        if(input[0]>='A' && input[0]<='H')
            converted.second = input[0]-65;			    //column index if uppercase letter

        else
            converted.first = -1;						//invalid


    return converted;
}





//the next 4 functions check whether the destination coordinates are diagonally connected to the source coordinates in the directions NE, SE, SW, NW respectively
bool first_check (pii from , pii to)
{
    if (from.first== to.first && from.second == to.second )
        return true;
    if (from.first < 0 && from.first > 7 || from.second < 0 || from.second > 7)
        return false ;
    return first_check({from.first -1 , from.second+1}, {to.first,to.second});
}
bool second_check (pii from , pii to)
{
    if (from.first== to.first && from.second == to.second )
        return true;
    if (from.first < 0 && from.first > 7 || from.second < 0 || from.second > 7)
        return false ;
    return second_check({from.first +1 , from.second+1}, {to.first,to.second});
}
bool third_check (pii from , pii to)
{
    if (from.first== to.first && from.second == to.second )
        return true;
     if (from.first < 0 && from.first > 7 || from.second < 0 || from.second > 7)
        return false ;
    return third_check({from.first +1 , from.second-1}, {to.first,to.second});
}
bool fourth_check (pii from , pii to)
{
    if (from.first== to.first && from.second == to.second )
        return true;
     if (from.first < 0 && from.first > 7 || from.second < 0 || from.second > 7)
        return false ;
    return fourth_check({from.first -1 , from.second-1}, {to.first,to.second});
}






//combines the last 4 functions in a single function
int diagonale_valid (pii from , pii to )
{
    if (first_check(from ,to) )                               // if 1 (x-1, y+1)
        return 1;
    else if (second_check(from , to))                         // if 2 (x+1 , y+1)
        return 2 ;
    else if(third_check(from , to))                           // if 3 (x+1, y-1)
        return 3;
    else if (fourth_check(from ,to))                          // if 4 (x-1, y-1)
        return 4;
    else
        return 0 ;                                            //  false
}



// function to check if source and destination coordinates are on same line horizontally or vertically
int same_line(pii from , pii to )
{
    if (from.first == to.first)
        return 1;                               // horizontal line
    else if(from.second == to.second)
        return -1 ;                             // vertical line
    else
        return 0 ;                              // false
}



// Function to check if any coordinates are free of any pieces
bool is_free (pii current )
{
    if (board[current.first][current.second].type == 0)
        return true;
    else
        return false ;
}



// function to generate next possible coordinates for each piece according to rules of chess
pair <int,int> generate_next_notations(pii from , pii to, pii next)
{
    if(board[from.first][from.second].type == 'Q')
    {

        if(diagonale_valid(from , to) > 0)
        {
            if(diagonale_valid(from , to)== 1)
                return {next.first -1 , next.second+1};
           else if(diagonale_valid(from , to)== 2)
                return {next.first +1 , next.second+1};
           else if(diagonale_valid(from , to)== 3)
               return {next.first +1 , next.second-1};
           else if(diagonale_valid(from , to)== 4)
               return {next.first -1 , next.second-1};
        }
         if(same_line(from , to) != 0)
         {
             if(same_line(next , to) == 1 && next.second > to.second)
                 return {next.first , next.second-1};
            else if (same_line(from , to) == 1 && from.second < to.second)
                return {next.first , next.second+1};
             else if(same_line(from , to) == -1 && from.first > to.first)
                 return {next.first-1 ,next.second};
             else if(same_line(from , to) == -1 && from.first < to.first)
                 return {next.first+1 , next.second};
         }
    }
    if(board[from.first][from.second].type == 'B')
    {
        if(diagonale_valid(from , to) != 0)
        {
            if(diagonale_valid(from , to)== 1)
                return {next.first -1 , next.second+1};
            else if(diagonale_valid(from , to)== 2)
                return {next.first +1 , next.second+1};
            else if(diagonale_valid(from , to)== 3)
                return {next.first +1 , next.second-1};
            else if(diagonale_valid(from , to)== 4)
                return {next.first -1 , next.second-1};
        }
    }
     if(board[from.first][from.second].type == 'R')
     {
         if(same_line(from , to) != 0)
         {
             if(same_line(from , to) == 1 && from.second > to.second)
                 return {next.first , next.second-1};
             else if (same_line(from , to) == 1 && from.second < to.second)
                 return {next.first , next.second+1};
             else if(same_line(from , to) == -1 && from.first > to.first)
                 return {next.first-1 ,next.second};
             else if(same_line(from , to) == -1 && from.first < to.first)
                 return {next.first+1 , next.second};
         }
     }
}

//checks if the suggested move of a queen piece is accepted or not according to chess rules
bool Queen_check(pii from , pii to)
{

    if(diagonale_valid(from , to) == 0 && same_line(from , to) == 0)
        return false;
    bool flag = true;
    pii next=from;

    while (flag && (next.first !=to.first || next.second != to.second ))
    {
        next=generate_next_notations(from,to,next);
        if (!is_free(next) && next!=to) //o2af abl a5er piece 3ashan nesta3mel nafs el function fe el eating
        {
            flag = false;
            break;
        }

    }
    if(flag)
        return true;
    else
        return false;
}

//checks if the suggested move of a rook piece is accepted or not according to chess rules
bool Rook_check(pii from , pii to)
{
    if(same_line (from,to)== 0)
        return false;
    bool flag = true;

    pii next=from;

    while (flag && (next.first !=to.first || next.second != to.second ))
    {
        next=generate_next_notations(from,to,next);
        if (!is_free(next) && next!=to)
        {
            flag = false;
            break;
        }

    }
    if(flag)
        return true;
    else
        return false;
}

//checks if the suggested move of a bishop piece is accepted or not according to chess rules
bool Bishop_check(pii from ,pii to )
{
    if(diagonale_valid(from , to) == 0 )
        return false;
    bool flag = true;
    pii next=from;

    while (flag && (next.first !=to.first || next.second != to.second ))
    {
        next=generate_next_notations(from,to,next);
        if (!is_free(next) && next!=to)
        {
            flag = false;
            break;
        }

    }
    if(flag)
        return true;
    else
        return false;

}

//checks if the suggested move of a knight piece is accepted or not according to chess rules
bool knight_check(pii from , pii to )
{
    if(from.first-to.first == 2 && from.second -to.second == 1)
        return true;
    else if (from.first-to.first == 2 && to.second -from.second == 1)
        return true ;
    else if(from.first-to.first == 1 && from.second -to.second == 2)
        return true ;
    else if (to.first-from.first == 1 && from.second -to.second == 2)
        return true ;
    else if (to.first-from.first == 2 && from.second -to.second == 1)
        return true;
    else if(to.first-from.first == 2 && to.second -from.second == 1)
        return true ;
    else if(to.first-from.first == 1 && to.second -from.second == 2)
        return true ;
    else if (from.first-to.first == 1 && to.second -from.second == 2)
        return true ;
    else
        return false ;
}

//next 2 functions check if the suggested move of a pawn piece is accepted or not according to chess rules
bool Pawn_check_vertical(pii from, pii to){

    if(from.second!=to.second ) return false;                                       //if from and to are not on same file (column) then move is automatically invalid


    if(board[from.first][from.second].color == 'w'){

        if(from.first-to.first== 2 && !board[from.first][from.second].moved)
            return true;                                                           //moving two spaces is only allowed for the first move of the pawn

        if(from.first-to.first==1)
            return true;                                                           //otherwise pawn can only move one space
    }
    else
    if(board[from.first][from.second].color == 'b'){

        if(to.first-from.first== 2 && !board[from.first][from.second].moved)
            return true;

        if(to.first-from.first==1)
            return true;
    }

    return false;
}

bool Pawn_check_diagonal(pii from, pii to){

    if(board[from.first][from.second].color == 'w'){                            //diagonal pawn moves can only be with a space of 1 square

        if(from.first-to.first==1 && abs(from.second-to.second)==1)             //abs indicates the possibility of both east and west diagonals
            return true;
    }
    else
    if(board[from.first][from.second].color == 'b'){

        if(to.first-from.first==1 && abs(to.second-from.second)==1)
            return true;
    }

    return false;
}

//checks if the suggested move of a king piece is accepted or not according to chess rules
bool King_check(pii from, pii to){
    if(abs(from.first-to.first)>1 ||abs(from.second-to.second)>1){return false;}
    return true;


}


//castling for white king
int castling_w(pii to)
{
    if(board[7][4].type=='K' && board[7][4].moved==0 && attacked_by_b[7][4]==0){

        if(to.first==7 && to.second==6){ // 7 6

            if(board[7][7].type=='R' && board[7][7].moved==0 && attacked_by_b[7][5]==0 && attacked_by_b[7][6]==0 && is_free({7,5}) && is_free({7,6}))
                return 1;
            else
                return 0;
        }

        if(to.first==7 && to.second==2){ // 7 2

            if(board[7][0].type=='R' && board[7][0].moved==0 && attacked_by_b[7][3]==0 && attacked_by_b[7][2]==0 && is_free({7,3}) && is_free({7,2}) && is_free({7,1}))
                return -1;
            else
                return 0;
        }
    }

    return 0;
}

//castling for black king
int castling_b(pii to)
{
    if(board[0][4].type=='K' && board[0][4].moved==0 && attacked_by_w[0][4]==0){

        if(to.first==0 && to.second==6){

            if(board[0][7].type=='R' && board[0][7].moved==0 && attacked_by_w[0][5]==0 && attacked_by_w[0][6]==0 && is_free({0,5}) && is_free({0,6}))
                return 1;
            else
                return 0;
        }

        if(to.first==0 && to.second==2){

            if(board[0][0].type=='R' && board[0][0].moved==0 && attacked_by_w[0][3]==0 && attacked_by_w[0][2]==0 && is_free({0,3}) && is_free({0,2}) && is_free({0,1}))
                return -1;
            else
                return 0;
        }
    }

    return 0;
}



//a function that checks if moving a piece is accepted or not and based on that decision it either moves the piece or refuses to do so
// if move=-1,-1(out of bounds) or if destination is not empty or  if source piece doesn't belong to player
// doesn't do anything and return false
//  else moves and return true
bool move_piece(pii x, pii y,int player){
    if(x.first==-1 || y.first==-1) return false;
    if(board[x.first][x.second].color!= (player==0?'w':'b') ) return false; //start piece not mine
    if(board[y.first][y.second].color==(player==0?'w':'b')) return false; //end piece mine


    //Castling
        if(player==0){
            if (x==w_king_pos){
                if(castling_w(y)==1) {
                board[x.first][x.second].moved=true;
                board[7][7].moved=true;
                swap(board[x.first][x.second], board[y.first][y.second]);
                swap(board[x.first][5], board[7][7]);

                w_king_pos = y;

                return true;
                }

                if(castling_w(y)==-1) {
                board[x.first][x.second].moved=true;
                board[7][0].moved=true;
                swap(board[x.first][x.second], board[y.first][y.second]);
                swap(board[x.first][x.second-1], board[7][0]);

                w_king_pos = y;

                return true;
                }
            }

        }

        if(player==1){
            if (x==b_king_pos){
                if(castling_b(y)==1) {
                board[x.first][x.second].moved=true;
                board[0][7].moved=true;
                swap(board[x.first][x.second], board[y.first][y.second]);
                swap(board[x.first][5], board[0][7]);

                b_king_pos = y;

                return true;
                }

                if(castling_b(y)==-1) {
                board[x.first][x.second].moved=true;
                board[7][0].moved=true;
                swap(board[x.first][x.second], board[y.first][y.second]);
                swap(board[x.first][x.second-1], board[7][0]);

                b_king_pos = y;

                return true;
                }
            }

        }

    //END CASTLING


    bool flag=true;
    if(board[y.first][y.second].color==(player==1?'w':'b')){ //end piece of opponent //eat code

        switch(board[x.first][x.second].type){
            case 'Q': flag=Queen_check(x,y);break;
            case 'R': flag=Rook_check(x,y);break;
            case 'N': flag=knight_check(x,y);break;
            case 'B':  flag=Bishop_check(x,y); break;
            case 'P': flag=Pawn_check_diagonal(x,y); break;
            case 'K': flag=King_check(x,y); break;
        }

        if(flag){
        board[x.first][x.second].moved=true;
         if(player==0 && board[x.first][x.second].type=='K'){
            w_king_pos=y;
         }  if(player==1 && board[x.first][x.second].type=='K'){
            b_king_pos=y;
         }
        swap(board[x.first][x.second],board[y.first][y.second]);


        board[x.first][x.second].type=0;
        board[x.first][x.second].color=0;
        board[x.first][x.second].moved=0;

        return true;
        }

        return false;

    }

    switch(board[x.first][x.second].type){
        case 'Q': flag=Queen_check(x,y);break;
        case 'R': flag=Rook_check(x,y);break;
        case 'N': flag=knight_check(x,y);break;
        case 'B':  flag=Bishop_check(x,y); break;
        case 'P': flag=Pawn_check_vertical(x,y); break;
        case 'K': flag=King_check(x,y); break;
    }

    if(flag){
        board[x.first][x.second].moved=true;
        if(player==0 && board[x.first][x.second].type=='K'){
            w_king_pos=y;
         }  if(player==1 && board[x.first][x.second].type=='K'){
            b_king_pos=y;
         }
        swap(board[x.first][x.second],board[y.first][y.second]);


        return true;
    }
    return false;
}

//sets the attacked by white array based on the type of piece it finds
void set_white(int r,int c){

    if(board[r][c].type=='P'){
        if(in_board(r-1,c+1))attacked_by_w[r-1][c+1]++;
        if(in_board(r-1,c-1))attacked_by_w[r-1][c-1]++;
    }

    if(board[r][c].type=='N'){
        if(in_board(r+1,c-2)) attacked_by_w[r+1][c-2]++;
        if(in_board(r-1,c-2)) attacked_by_w[r-1][c-2]++;
        if(in_board(r+1,c+2)) attacked_by_w[r+1][c+2]++;
        if(in_board(r-1,c+2)) attacked_by_w[r-1][c+2]++;
        if(in_board(r+2,c+1)) attacked_by_w[r+2][c+1]++;
        if(in_board(r+2,c-1)) attacked_by_w[r+2][c-1]++;
        if(in_board(r-2,c+1)) attacked_by_w[r-2][c+1]++;
        if(in_board(r-2,c-1)) attacked_by_w[r-2][c-1]++;
    }

    if(board[r][c].type=='K'){// TO BE MODIFIED
            for(int i=-1; i<=1; i++){
                for(int j=-1; j<=1; j++){
                    if(i==0 && j==0) continue;
                    else{
                        if(in_board(r+i,c+j))
                        attacked_by_w[r+i][c+j]++;
                    }
                }
            }

    }

    if(board[r][c].type=='R' || board[r][c].type=='Q'){

        for(int i=r; i<8; i++){
            if(i==r) continue;
            if(is_free({i,c})) attacked_by_w[i][c]++;
            else {
                attacked_by_w[i][c]++; break;
            }
        }

        for(int i=r; i>=0; i--){
            if(i==r) continue;
            if(is_free({i,c})) attacked_by_w[i][c]++;
             else {
                 attacked_by_w[i][c]++; break;
            }
        }
        for(int i=c; i<8; i++){
            if(i==c) continue;
            if(is_free({r,i})) attacked_by_w[r][i]++;
             else {
                attacked_by_w[r][i]++; break;
            }
        }
        for(int i=c; i>=0; i--){
             if(i==c) continue;
            if(is_free({r,i}))attacked_by_w[r][i]++;
             else {
                attacked_by_w[r][i]++; break;
            }
        }
    }

    if(board[r][c].type=='B'|| board[r][c].type=='Q'){

        for(int i=r, j=c; i<8 && j<8; i++, j++){
            if(i==r && j==c) continue;
            if(is_free({i,j})){
                attacked_by_w[i][j]++;
            }else{
                 attacked_by_w[i][j]++; break;
            }

        }


        for(int i=r,j=c; i<8 && j>=0; i++, j--){
           if(i==r && j==c) continue;
            if(is_free({i,j})){
                attacked_by_w[i][j]++;
            }else{
                 attacked_by_w[i][j]++; break;
            }

        }

        for(int i=r, j=c; i>=0 && j<8; i--, j++){
           if(i==r && j==c) continue;
            if(is_free({i,j})){
                attacked_by_w[i][j]++;
            }else{
                 attacked_by_w[i][j]++; break;
            }

        }

        for(int i=r, j=c; i>=0 && j>=0; i--, j--){
            if(i==r && j==c) continue;
            if(is_free({i,j})){
                attacked_by_w[i][j]++;
            }else{
                 attacked_by_w[i][j]++; break;
            }

        }


    }

}

//sets the attacked by black array based on the type of piece it finds
void set_black(int r,int c){
    if(board[r][c].type=='P'){
        if(in_board(r+1,c-1))attacked_by_b[r+1][c-1]++;
        if(in_board(r+1,c+1))attacked_by_b[r+1][c+1]++;
    }

    if(board[r][c].type=='N'){
        if(in_board(r+1,c-2)) attacked_by_b[r+1][c-2]++;
        if(in_board(r-1,c-2)) attacked_by_b[r-1][c-2]++;
        if(in_board(r+1,c+2)) attacked_by_b[r+1][c+2]++;
        if(in_board(r-1,c+2)) attacked_by_b[r-1][c+2]++;
        if(in_board(r+2,c+1)) attacked_by_b[r+2][c+1]++;
        if(in_board(r+2,c-1)) attacked_by_b[r+2][c-1]++;
        if(in_board(r-2,c+1)) attacked_by_b[r-2][c+1]++;
        if(in_board(r-2,c-1)) attacked_by_b[r-2][c-1]++;
    }

    if(board[r][c].type=='K'){// TO BE MODIFIED
            for(int i=-1; i<=1; i++){
                for(int j=-1; j<=1; j++){
                    if(i==0 && j==0) continue;
                    else{
                         if(in_board(r+i,c+j))
                        attacked_by_b[r+i][c+j]++;
                    }
                }
            }

    }



    if(board[r][c].type=='R' || board[r][c].type=='Q'){

        for(int i=r; i<8; i++){
            if(i==r) continue;
            if(is_free({i,c})) attacked_by_b[i][c]++;
            else {
                attacked_by_b[i][c]++; break;
            }
        }

        for(int i=r; i>=0; i--){
            if(i==r) continue;
            if(is_free({i,c})) attacked_by_b[i][c]++;
             else {
                 attacked_by_b[i][c]++; break;
            }
        }
        for(int i=c; i<8; i++){
            if(i==c) continue;
            if(is_free({r,i})) attacked_by_b[r][i]++;
             else {
                attacked_by_b[r][i]++; break;
            }
        }
        for(int i=c; i>=0; i--){
             if(i==c) continue;
            if(is_free({r,i}))attacked_by_b[r][i]++;
             else {
                attacked_by_b[r][i]++; break;
            }
        }
    }

    if(board[r][c].type=='B'|| board[r][c].type=='Q'){

        for(int i=r, j=c; i<8 && j<8; i++, j++){
            if(i==r && j==c) continue;
            if(is_free({i,j})){
                attacked_by_b[i][j]++;
            }else{
                 attacked_by_b[i][j]++; break;
            }

        }


        for(int i=r,j=c; i<8 && j>=0; i++, j--){
           if(i==r && j==c) continue;
            if(is_free({i,j})){
                attacked_by_b[i][j]++;
            }else{
                 attacked_by_b[i][j]++; break;
            }

        }

        for(int i=r, j=c; i>=0 && j<8; i--, j++){
           if(i==r && j==c) continue;
            if(is_free({i,j})){
                attacked_by_b[i][j]++;
            }else{
                 attacked_by_b[i][j]++; break;
            }

        }

        for(int i=r, j=c; i>=0 && j>=0; i--, j--){
            if(i==r && j==c) continue;
            if(is_free({i,j})){
                attacked_by_b[i][j]++;
            }else{
                 attacked_by_b[i][j]++; break;
            }

        }


    }

}

//calls set white when if finds a piece
void set_attacked_by_w(){


    lp(i,8){
        lp(j,8){
            attacked_by_w[i][j]=0;

            }

    }


    lp(i,8){
        lp(j,8){


            if(board[i][j].color=='w'){
                set_white(i,j);


            }


        }

    }

}

//calls set black when if finds a piece
void set_attacked_by_b(){
    lp(i,8){
        lp(j,8){
            attacked_by_b[i][j]=0;

            }
    }

    lp(i,8){
        lp(j,8){
            if(board[i][j].color=='b'){
                set_black(i,j);
            }

        }

    }

}

//see if white king is checked
bool check_w(){
    pii temp=w_king_pos;
    if(attacked_by_b[w_king_pos.first][w_king_pos.second]!=0){
        return true;
    }
    return false;


}

//see if black king is checked
bool check_b(){
     pii temp=b_king_pos;
    if(attacked_by_w[b_king_pos.first][b_king_pos.second]!=0){
        return true;
    }
    return false;


}

//set the temporary arrays and values with the current values
void set_temps(int x=1){
    if(x==1){
    lp(i,8){
        lp(j,8){
         temp_board[i][j]=board[i][j];
         temp_attacked_by_w[i][j]=attacked_by_w[i][j];

         temp_attacked_by_b[i][j]=attacked_by_b[i][j];
        }
    }

    temp_w_king_pos=w_king_pos;
    temp_b_king_pos=b_king_pos;
    }
    if(x==2){
            lp(i,8){
        lp(j,8){
         temp_board2[i][j]=board[i][j];
         temp_attacked_by_w2[i][j]=attacked_by_w[i][j];

         temp_attacked_by_b2[i][j]=attacked_by_b[i][j];
        }
    }

    temp_w_king_pos2=w_king_pos;
    temp_b_king_pos2=b_king_pos;

    }
}

//set the current values from the saved ones (reverse process of set_temps())
void temps_to_main(int x=1){
    if(x==1){
    lp(i,8){
        lp(j,8){
         board[i][j]=temp_board[i][j];
         attacked_by_w[i][j]=temp_attacked_by_w[i][j];

         attacked_by_b[i][j]=temp_attacked_by_b[i][j];
        }
    }
    w_king_pos=temp_w_king_pos;
    b_king_pos=temp_b_king_pos;
    }
    if(x==2){

        lp(i,8){
            lp(j,8){
             board[i][j]=temp_board2[i][j];
             attacked_by_w[i][j]=temp_attacked_by_w2[i][j];

             attacked_by_b[i][j]=temp_attacked_by_b2[i][j];
            }
        }
        w_king_pos=temp_w_king_pos2;
        b_king_pos=temp_b_king_pos2;


    }
}

//check if white has any acceptable moves (that is accepted and where king is not checked)
bool can_move_w(){
    bool flag;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            for(int x=0; x<8; x++){
                for(int y=0; y<8; y++){
                    set_temps();
                    flag=move_piece({i,j},{x,y},0);


                    set_attacked_by_w();
                    set_attacked_by_b();
                    if( check_w()) flag=false;
                     temps_to_main();

                     if(flag==true) return true;


                }

            }
        }

    }


    return false;
}

//check if black has any acceptable moves (that is accepted and where king is not checked)
bool can_move_b(){
    bool flag;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            for(int x=0; x<8; x++){
                for(int y=0; y<8; y++){
                    set_temps();
                    flag=move_piece({i,j},{x,y},1);
                    set_attacked_by_w();
                    set_attacked_by_b();
                    if( check_b()) {
                            flag=false;
                    }
                     temps_to_main();

                     if(flag==true){

                        return true;
                     }


                }

            }
        }

    }


    return false;
}

//everything related to pawn promotion
void pawn_promotion(int player)
{
    int tempx;
    bool flag=false;
    SDL_Window* promotion_window=NULL;
    SDL_Surface* promotion_surface=NULL;
    SDL_Surface* promotion_pic=NULL;

    if(player==0){

        for(int i=0;i<8;i++)
            if(board[0][i].type=='P'){
                tempx=i;
                flag=true;
                break;
            }

        if(!flag)
            return;

        promotion_window = SDL_CreateWindow("PAWN PROMOTION", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 419, 160, SDL_WINDOW_SHOWN);
        SDL_SetWindowIcon(promotion_window,icon);
        promotion_surface = SDL_GetWindowSurface(promotion_window);
        promotion_pic = SDL_LoadBMP("Chess Piece Icons/pawnpromotew.bmp");

        SDL_BlitSurface(promotion_pic,NULL,promotion_surface,NULL);

        SDL_UpdateWindowSurface(promotion_window);

        SDL_Event e;
        while(SDL_PollEvent(&e) || true){
                int x,y;
                SDL_GetMouseState(&x,&y);

                if(e.type==SDL_MOUSEBUTTONUP){
                    if(SDL_GetMouseFocus()!=promotion_window) continue;
                    SDL_GetMouseState(&x,&y);

                    if(x>=30 && x<=90 && y>=60 && y<=120){

                        SDL_DestroyWindow(promotion_window);
                        promotion_window = NULL;

                        board[0][tempx].type='Q';

                        if(boardcolor[0][tempx]=='w')
                            custom_blit(col_to_pixel(tempx),row_to_pixel(0),tile_w,mainsurface);
                        else
                            custom_blit(col_to_pixel(tempx),row_to_pixel(0),tile_b,mainsurface);

                        custom_blit(col_to_pixel(tempx),row_to_pixel(0),queen_sprite_w,mainsurface);

                        break;
                    }

                    if(x>=125 && x<=190 && y>=60 && y<=120){
                        SDL_DestroyWindow(promotion_window);
                        promotion_window = NULL;

                        board[0][tempx].type='N';

                        if(boardcolor[0][tempx]=='w')
                            custom_blit(col_to_pixel(tempx),row_to_pixel(0),tile_w,mainsurface);
                        else
                            custom_blit(col_to_pixel(tempx),row_to_pixel(0),tile_b,mainsurface);

                        custom_blit(col_to_pixel(tempx),row_to_pixel(0),knight_sprite_w,mainsurface);

                        break;
                    }

                    if(x>=230 && x<=290 && y>=60 && y<=120){

                        SDL_DestroyWindow(promotion_window);
                        promotion_window = NULL;

                        board[0][tempx].type='R';

                        if(boardcolor[0][tempx]=='w')
                            custom_blit(col_to_pixel(tempx),row_to_pixel(0),tile_w,mainsurface);
                        else
                            custom_blit(col_to_pixel(tempx),row_to_pixel(0),tile_b,mainsurface);

                        custom_blit(col_to_pixel(tempx),row_to_pixel(0),rook_sprite_w,mainsurface);

                        break;
                    }

                    if(x>=325 && x<=385 && y>=60 && y<=120){

                        SDL_DestroyWindow(promotion_window);
                        promotion_window = NULL;

                        board[0][tempx].type='B';

                        if(boardcolor[0][tempx]=='w')
                            custom_blit(col_to_pixel(tempx),row_to_pixel(0),tile_w,mainsurface);
                        else
                            custom_blit(col_to_pixel(tempx),row_to_pixel(0),tile_b,mainsurface);

                        custom_blit(col_to_pixel(tempx),row_to_pixel(0),bishop_sprite_w,mainsurface);

                        break;
                    }


                }
                e.type=NULL;
        }

    }else
    {

        for(int i=0;i<8;i++)
            if(board[7][i].type=='P'){
                tempx=i;
                flag=true;
                break;
            }

        if(!flag)
            return;

        promotion_window = SDL_CreateWindow("PAWN PROMOTION", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 419, 160, SDL_WINDOW_SHOWN);
        promotion_surface = SDL_GetWindowSurface(promotion_window);
        promotion_pic = SDL_LoadBMP("Chess Piece Icons/pawnpromoteb.bmp");

        SDL_BlitSurface(promotion_pic,NULL,promotion_surface,NULL);

        SDL_UpdateWindowSurface(promotion_window);

        SDL_Event e;
        while(SDL_PollEvent(&e) || true){
                int x,y;
                if(e.type==SDL_MOUSEBUTTONUP){
                    if(SDL_GetMouseFocus()!=promotion_window) continue;
                    SDL_GetMouseState(&x,&y);

                    if(x>=30 && x<=90 && y>=60 && y<=120){

                        SDL_DestroyWindow(promotion_window);
                        promotion_window = NULL;

                        board[7][tempx].type='Q';

                        if(boardcolor[7][tempx]=='w')
                            custom_blit(col_to_pixel(tempx),row_to_pixel(7),tile_w,mainsurface);
                        else
                            custom_blit(col_to_pixel(tempx),row_to_pixel(7),tile_b,mainsurface);

                        custom_blit(col_to_pixel(tempx),row_to_pixel(7),queen_sprite_b,mainsurface);

                        break;
                    }

                    if(x>=125 && x<=190 && y>=60 && y<=120){
                        SDL_DestroyWindow(promotion_window);
                        promotion_window = NULL;

                        board[7][tempx].type='N';

                        if(boardcolor[7][tempx]=='w')
                            custom_blit(col_to_pixel(tempx),row_to_pixel(7),tile_w,mainsurface);
                        else
                            custom_blit(col_to_pixel(tempx),row_to_pixel(7),tile_b,mainsurface);

                        custom_blit(col_to_pixel(tempx),row_to_pixel(7),knight_sprite_b,mainsurface);

                        break;
                    }

                    if(x>=230 && x<=290 && y>=60 && y<=120){

                        SDL_DestroyWindow(promotion_window);
                        promotion_window = NULL;

                        board[7][tempx].type='R';

                        if(boardcolor[7][tempx]=='w')
                            custom_blit(col_to_pixel(tempx),row_to_pixel(7),tile_w,mainsurface);
                        else
                            custom_blit(col_to_pixel(tempx),row_to_pixel(7),tile_b,mainsurface);

                        custom_blit(col_to_pixel(tempx),row_to_pixel(7),rook_sprite_b,mainsurface);

                        break;
                    }

                    if(x>=325 && x<=385 && y>=60 && y<=120){

                        SDL_DestroyWindow(promotion_window);
                        promotion_window = NULL;

                        board[7][tempx].type='B';

                        if(boardcolor[7][tempx]=='w')
                            custom_blit(col_to_pixel(tempx),row_to_pixel(7),tile_w,mainsurface);
                        else
                            custom_blit(col_to_pixel(tempx),row_to_pixel(7),tile_b,mainsurface);

                        custom_blit(col_to_pixel(tempx),row_to_pixel(7),bishop_sprite_b,mainsurface);

                        break;
                    }


                }
                e.type=NULL;
        }

    }

    return;
}

//GUI windows of the pre-game
void decision_taking(bool *norm , bool *fun , bool *pvsai , bool *pvp)
{
    SDL_Window* decision_window = NULL;
    SDL_Surface* mainsurface_decision = NULL;

    SDL_Surface* decisionpic1 = SDL_LoadBMP("Chess Piece Icons/decision1.bmp");
    SDL_Surface* decisionpic2 = SDL_LoadBMP("Chess Piece Icons/decision2.bmp");

    decision_window = SDL_CreateWindow("CHESS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 551, 376, SDL_WINDOW_SHOWN);
    SDL_SetWindowIcon(decision_window,icon);
    mainsurface_decision = SDL_GetWindowSurface(decision_window);

    while(1)
    {
        *norm=false; *fun=false; *pvsai=false; *pvp=false;
        SDL_BlitSurface(decisionpic1,NULL,mainsurface_decision,NULL);
        SDL_UpdateWindowSurface(decision_window);

        SDL_Event e;
        while(SDL_PollEvent(&e) || true){
                int x,y;

                if(e.type==SDL_MOUSEBUTTONUP){

                if(SDL_GetMouseFocus()!=decision_window)
                        continue;

                    SDL_GetMouseState(&x,&y);

                    if(x>=147 && x<=407 && y>=72 && y<=155){

                        *norm=true;
                        e.type=NULL;
                        break;
                    }

                    else if(x>=147 && x<=407 && y>=204 && y<=289){

                        *fun=true;
                        e.type=NULL;
                        break;
                    }
                    e.type=NULL;
                }

                e.type=NULL;

        }

       // decisionpic2 = SDL_LoadBMP("Chess Piece Icons/n.bmp");


        SDL_BlitSurface(decisionpic2,NULL,mainsurface_decision,NULL);
        SDL_UpdateWindowSurface(decision_window);

        SDL_Event f;
        bool backbutton=false;
        while(SDL_PollEvent(&f) || true){
                int m,n;
                if(f.type==SDL_MOUSEBUTTONUP){

                    if(SDL_GetMouseFocus()!=decision_window)
                        continue;

                    SDL_GetMouseState(&m,&n);

                    if(m>=147 && n<=407 && m>=72 && n<=155){

                        *pvsai=true;
                        f.type=NULL;
                        break;
                    }

                    if(m>=147 && m<=407 && n>=204 && n<=289){

                        *pvp=true;
                        f.type=NULL;
                        break;
                    }

                    if(m>=18 && m<=118 && n>=319 && n<=360){

                        backbutton=true;
                        f.type=NULL;
                        break;
                    }
                }
        }


        if(!backbutton)
            break;

    }//end while(1)

    SDL_DestroyWindow(decision_window);
    decision_window = NULL;

    return;
}
//prints in console if a specific player's king is checked
void print_check(){

            if(check_w()){
                cout<<"-----------\n";
                cout<<"CHECK WHITE" <<endl;
            cout<<"-----------\n";
            }

            if(check_b()){
                cout<<"-----------\n";
                cout<<"CHECK BLACK"<<endl;
                cout<<"-----------\n";
            }
}

//prints in console if a specific player wins by checkmate
int game_over(int player){
    if(player==0){
                    if(!can_move_w()){
                        if(check_w()){
                                cout<<"CHECK MATE WHITE -- BLACK WINS" <<endl;
                            return 1;
                       } else {
                            cout<<"STALEMATE"<<endl;
                        return -1;
                        }

                    }
    }else{
                if(!can_move_b()){
                        if(check_b()){
                            cout<<"CHECK MATE BLACK -- WHITE WINS" <<endl;
                            return 1;
                        }else {
                                                       cout<<"STALEMATE"<<endl;

                            return -1;
                        }

                    }

    }
    return 0;
}

//checks if suggested move is valid or not. state=0 means suggested move will be applied and the opposite for state=1
bool valid_move(pii from, pii to, int player,int state=1){

        bool flag;

        set_temps();
        flag=move_piece(from,to,player);
        set_attacked_by_w();
        set_attacked_by_b();

        if(player==0 && check_w())
            flag=false;

        if(player==1 && check_b())
            flag=false;


        if(flag==false|| state==0)
            temps_to_main();

        return flag;
}

//empties all the temps declared earlier
void clean_up(){

    lp(i,8){
        lp(j,8){
            attacked_by_w[i][j]=0;
             attacked_by_b[i][j]=0;
             temp_attacked_by_w[i][j]=0;
             temp_attacked_by_b[i][j]=0;
        }
    }
}

//closes the main GUI window
void close()
{
	//Destroy window
	SDL_DestroyWindow( window );

	//frees memory
	window = NULL;
}

//if yes button return 1 if no return -1 other return 0
int in_button(int x,int y){

    if(x>=230 && x<=285 && y>=120 && y<= 149){
        return 1;

    }

    if(x>=140 && x<=200 && y>=120 && y<= 149){
        return -1;

    }
    return 0;
}

//sets value of each piece to help our AI estimate his moves
long long int evaluate_value(){
    long long int val=0;
    const int val_Pawn=1;
        const int val_Knight=3  ;
        const int val_Bishop=3;

        const int val_Rook=5;

        const int val_Queen=90;


        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                val+=(attacked_by_b[i][j]!=0);
                val-=(attacked_by_w[i][j]!=0);
                if(board[i][j].type=='P'){
                    if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Pawn*20;
                    if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Pawn*10;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Pawn*2;

                }
                if(board[i][j].type=='B'){
                      if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Bishop*10;
                      if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Bishop;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Bishop*2;
                      if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val-=val_Bishop;
                }
                if(board[i][j].type=='R'){
                      if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Rook*10;
                        if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Rook;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Rook*2;
                        if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val-=val_Rook;
                }
                if(board[i][j].type=='N'){
                      if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Rook*10;
                        if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Rook;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Rook*2;
                        if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val-=val_Rook;
                }
                if(board[i][j].type=='Q'){
                    if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Queen*20;
                    if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Queen*50;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Queen;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val-=val_Queen;
                }

        }
        }

        if(check_w()){
            val+=10;
        }
        if(check_b()){
            val-=10;
        }

        if(!can_move_w()){
            val+=10000;
        }




        return val;






}


//the same as the first one but for minimax algorithm
long long int evaluate_value2(){
    long long int val=0;
    const int val_Pawn=1;
        const int val_Knight=3  ;
        const int val_Bishop=3;

        const int val_Rook=5;

        const int val_Queen=90;


        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                val+=(attacked_by_b[i][j]!=0)*2;
                val-=(attacked_by_w[i][j]!=0)*2;
                if(board[i][j].type=='P'){
                    if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Pawn*5;
                    if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Pawn*2;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Pawn*5;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val+=val_Pawn*2;

                }
                if(board[i][j].type=='B'){
                      if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Bishop*10;
                      if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Bishop*5;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Bishop*10;
                      if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val+=val_Bishop*5;
                }
                if(board[i][j].type=='R'){
                      if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Rook*10;
                        if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Rook*5;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Rook*10;
                        if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val+=val_Rook*5;
                }
                if(board[i][j].type=='N'){
                      if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Rook*10;
                        if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Rook*5;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Rook*10;
                        if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val+=val_Rook*5;
                }
                if(board[i][j].type=='Q'){
                    if(board[i][j].color=='b' && attacked_by_w[i][j]==0) val+=val_Queen*20;
                    if(board[i][j].color=='b' && attacked_by_w[i][j]!=0) val-=val_Queen*50;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]==0) val-=val_Queen*20;
                    if(board[i][j].color=='w' && attacked_by_b[i][j]!=0) val+=val_Queen*50;
                }

        }
        }

        if(check_w()){
            val+=20;
        }
        if(check_b()){
            val-=20;
        }

        if(!can_move_w()){
            val+=10000;
        }


        if(!can_move_b()){
            val-=100000;
        }


        return val;






}




//copies current board configuration to the AI decision making process
void copy_to_boardAI(vector<vector<piece>> &boardAI){

    lp(i,8){
        lp(j,8){
            boardAI[i][j]=board[i][j];


        }

    }



}

//copies current board configuration from the AI decision making process
void copy_from_boardAI(vector<vector<piece>> &boardAI){

    lp(i,8){
        lp(j,8){
            board[i][j]=boardAI[i][j];
        }
    }

    set_attacked_by_w();
    set_attacked_by_b();

}

//minimax algorithm
pair< pair<long long int,int>, pair<int,int> > best_move_min_max(vector<vector<piece>> boardAI, int depth,int player, bool first_call){
    pii from,to;
    bool flag;
    long long int val=0;
auto temp=boardAI;
    if(depth==0){

            if (player==1){
                val=-1000000;
                for(int i=0; i<8; i++){
                    for(int j=0; j<8; j++){
                            if(boardAI[i][j].color!='b') continue;
                        for(int x=0; x<8; x++){
                            for(int y=0; y<8; y++){
                                set_temps(2);
                                boardAI=temp;
                                copy_from_boardAI(boardAI);
                                flag=valid_move({i,j}, {x,y}, 1,1);


                                if(flag){

                                     auto t=evaluate_value2();

                                    if(t>val){
                                        val=t;
                                        from=mp(i,j);
                                        to=mp(x,y);
                                    }

                                }
                                temps_to_main(2);


                            }
                        }
                    }
                }


            }


            if (player==0){
                val=1000000;
                for(int i=0; i<8; i++){
                    for(int j=0; j<8; j++){
                        if(boardAI[i][j].color!='w') continue;
                        for(int x=0; x<8; x++){
                            for(int y=0; y<8; y++){
                                set_temps(2);
                                 boardAI=temp;
                                copy_from_boardAI(boardAI);
                                flag=valid_move({i,j},{x,y},0);
                                copy_to_boardAI(boardAI);
                                if(flag){

                                auto t=evaluate_value2();


                                    if(t<val){
                                        val=t;
                                        from=mp(i,j);
                                        to=mp(x,y);
                                    }
                                }
                                temps_to_main(2);


                            }
                        }
                    }
                }



            }





    }else{

            if (player==1){
                 val=-1000000;
                for(int i=0; i<8; i++){
                    for(int j=0; j<8; j++){
                        if(boardAI[i][j].color!='b') continue;
                        for(int x=0; x<8; x++){
                            for(int y=0; y<8; y++){
                                set_temps(2);
                                boardAI=temp;
                                copy_from_boardAI(boardAI);
                                flag=valid_move({i,j}, {x,y}, 1,1);
                                copy_to_boardAI(boardAI);

                                temps_to_main(2);

                                if(flag){

                                     auto t=best_move_min_max(boardAI, depth-1, 0,false);

                                    if(t.first.first>val){
                                        val=t.first.first;
                                        from=mp(i,j);
                                        to=mp(x,y);
                                    }

                                }


                            }
                        }
                    }
                }



            }

            if (player==0){
                 val=1000000;
                for(int i=0; i<8; i++){
                    for(int j=0; j<8; j++){
                             if(boardAI[i][j].color!='w') continue;
                        for(int x=0; x<8; x++){
                            for(int y=0; y<8; y++){
                                set_temps(2);
                                 boardAI=temp;
                                copy_from_boardAI(boardAI);
                                flag=valid_move({i,j},{x,y},0);
                                copy_to_boardAI(boardAI);
                                temps_to_main(2);
                                if(flag){
                                auto t=best_move_min_max(boardAI, depth-1, 1,false);

                                    if(t.first.first<val){
                                        val=t.first.first;
                                        from=mp(i,j);
                                        to=mp(x,y);
                                    }
                                }

                            }
                        }
                    }
                }



            }
    }


    if(first_call)
    return {from,to};
    else return {{val,-1},{-1,-1}};



}






//best move for normal AI
pair< pair<int,int>, pair<int,int> > best_move(){
    long long int val=-100000000;
    bool flag;
    pii from, to;
    srand(time(NULL));
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(board[i][j].color!='b') continue;

            for(int x=0; x<8; x++){
                for(int y=0; y<8; y++){
                    //if()
                    set_temps(2);
                    flag=move_piece({i,j},{x,y},1);
                    set_attacked_by_w();
                    set_attacked_by_b();
                    if( check_b()) {
                            flag=false;
                    }

                     if(flag==true){

                        long long int nx=evaluate_value();
                        if(nx>=val){
                            val=nx;
                            from=mp(i,j); to=mp(x,y);

                        }
                     }
                      temps_to_main(2);




                }

            }
        }

    }




    return {from,to};


}


//MAIN FUNCTION
int main (int argc, char* args[])
{

    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);

    print_logo();

    bool game_ended=false;

    //decision taking
    bool norm,fun, pvsai,pvp;
    decision_taking(&norm , &fun , &pvsai ,&pvp);

    while(game_ended==false){

        clean_up();
        init_gui();

        if(fun){
             reset_board_funmode();
            reset_board_gui_funmode();

        }else{

        reset_board_gui();
        reset_board();
        }

        string from,to;
        int player=0;

        while(1){


            cout<<"\n\n\n\n";

            //print_board(board);
            print_check(); //prints if either player is in check

            if(game_over(player)!=0){
                break;
            }

            bool flag;

            do{

                SDL_UpdateWindowSurface(window);

                cout<<"\nPlayer "<<player+1<<"'s Turn: \n";

                bool quit = false;
                SDL_Event e;
                pii m[2];
                int idx=0;

                if(pvsai){
                    if(player==1){
                        pair< pair<int,int>, pair<int,int> > x;
                        vector<vector<piece>> boardAI(8);
                        lp(i,8){
                            lp(j,8){
                                boardAI[i].push_back(board[i][j]);

                            }


                        }


                        //x=best_move_min_max(boardAI,1,1,true);
                        x=best_move();
                        m[0]=x.first;
                        m[1]=x.second;
                        idx=2;
                    }

                    while( idx!=2 && (SDL_PollEvent( &e ) || true ))
                    {

                            int x,y;
                            SDL_GetMouseState(&x,&y);

                            if(e.type==SDL_MOUSEBUTTONUP){

                                    m[idx].first=pixel_to_row(y);
                                    m[idx].second=pixel_to_col(x);

                                    if(in_board(m[idx].first,m[idx].second)){
                                        idx+=1;
                                    }


                            }
                            e.type=NULL;

                            if(idx>=2)  {break;}

                    }

                }else{

                        while( SDL_PollEvent( &e ) || true )
                        {

                            int x,y;
                            SDL_GetMouseState(&x,&y);

                            if(e.type==SDL_MOUSEBUTTONUP){

                                    m[idx].first=pixel_to_row(y);
                                    m[idx].second=pixel_to_col(x);

                                    if(in_board(m[idx].first,m[idx].second)){
                                        idx+=1;
                                    }


                            }
                            e.type=NULL;

                            if(idx>=2)  {break;}

                        }


                }

                cout<<"MOVING PIECE"<<endl;
                cout<<m[0].first<<" "<<m[0].second<<" ----> "<<m[1].first<<" "<<m[1].second<<endl;

                flag=valid_move(m[0],m[1],player,0);


                if(flag){
                                cout<<"VALID MOVE"<<endl;

                                move_piece_gui(m[0],m[1]);
                                valid_move(m[0],m[1],player,1);
                                if (!fun) pawn_promotion(player);
                                SDL_UpdateWindowSurface(window);
                }

            }while(flag==false);


            player++;
            player%=2;

        }

        cout<<"GAME HAS ENDED!";

        //START POST-GAME CHOICE WINDOW

        SDL_Event e;
        continue_window = SDL_CreateWindow("GAME OVER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 419, 160, SDL_WINDOW_SHOWN);
        continue_window_surface = SDL_GetWindowSurface(continue_window);
        continue_window_image=SDL_LoadBMP("Chess Piece Icons/yesnomsgbox.bmp");
        SDL_BlitSurface(continue_window_image,NULL,continue_window_surface,NULL);
        SDL_UpdateWindowSurface(continue_window);


        while(SDL_PollEvent(&e) || true){
                int x,y;

                if(e.type==SDL_MOUSEBUTTONUP){
                    if(SDL_GetMouseFocus()!=continue_window) continue;
                    SDL_GetMouseState(&x,&y);

                    if(in_button(x,y)==-1){


                        game_ended=true;
                        break;
                    }

                    if(in_button(x,y)==1){


                        game_ended=false;
                        break;
                    }


                }
                e.type=NULL;

        }
        SDL_DestroyWindow( continue_window );
        continue_window = NULL;
        close();

        //END POST-GAME CHOICE WINDOW

        if(game_ended==false){
            cout<<"\n\n\n\n\n\n\n|---------------------- NEW GAME ----------------------|\n\n\n\n\n\n\n";
        }

    }

    SDL_Quit();
    return 0;
}
//ALL CREDIT GOES TO: MOHAMED AHMED NABIL - MOHAMED YASSER ABDEL KADER - ANDREW ASHRAF
//END OF PROGRAM
