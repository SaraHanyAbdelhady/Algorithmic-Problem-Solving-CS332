#include <iostream>
#include <vector>
#define EMPTY 0
#define WHITE_KNIGHT 1 
#define BLACK_KNIGHT 2 
#define ROWS 4
#define COLUMNS 3
#define TOP_LEFT 1
#define TOP_CENTRE 2
#define TOP_RIGHT 3
#define BOTTOM_LEFT 10
#define BOTTOM_CENTRE 11
#define BOTTOM_RIGHT 12
#define NO_PIECES 12
class Board
{
    private:
        int counter; //counter to hold the number of moves
        std::vector<std::vector<int>> tiles; //2d matrix for the board
        std::vector<bool> occupied;
        std::vector<std::vector<int>> graph; //2d matrix that has possible movies
    public:
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
        //OVERLOADING THE << OPERATOR TO PRINT THE BOARD
        Board()
        {
            counter=0;
            tiles=std::vector<std::vector<int>>{
            {BLACK_KNIGHT,BLACK_KNIGHT,BLACK_KNIGHT}
            ,{EMPTY,EMPTY,EMPTY}
            ,{EMPTY,EMPTY,EMPTY}
            ,{WHITE_KNIGHT,WHITE_KNIGHT,WHITE_KNIGHT}
            };
            graph=std::vector<std::vector<int>>{
                {6,8},
                {7,9},
                {4,8},
                {3,9,11},
                {10,12},
                {1,7,11},
                {2,6,12},
                {1,3},
                {2,4,10},
                {5,9},
                {4,6},
                {5,7}
            };
            occupied=std::vector<bool>{
                true,true,true,
                false,false,false,
                false,false,false,
                true,true,true,
            };
        }
        std::vector<int> convertTo2dIndex(int i)
        {
            int n=i-1;
            return std::vector<int>{n/COLUMNS,(n%COLUMNS)};
        }
        bool isThatTileOccupied(int i)
        {
            auto indices = convertTo2dIndex(i);
            if(tiles[indices[0]][indices[1]]!=EMPTY)
                return true ;
            return false;
        } 
        bool isThatTileWhite(int i)
        {
            auto indices = convertTo2dIndex(i);
            if(tiles[indices[0]][indices[1]]==WHITE_KNIGHT)
                return true ;
            return false;
        }
        
        bool isThatTileBlack(int i)
        {
            auto indices = convertTo2dIndex(i);
            if(tiles[indices[0]][indices[1]]==BLACK_KNIGHT)
                return true ;
            return false;
        }
        bool hasReachedFinal()
        {
            for(int j=0;j<COLUMNS;j++)
            {
                if(tiles[0][j]!=WHITE_KNIGHT || tiles[3][j]!=BLACK_KNIGHT)
                    return false;
            }
            return true;
        }
        bool hasReachedStaleMate()
       {
           std::vector<std::vector<int>> stalemate{
            {WHITE_KNIGHT,EMPTY,WHITE_KNIGHT}
            ,{EMPTY,EMPTY,EMPTY}
            ,{BLACK_KNIGHT,EMPTY,EMPTY}
            ,{BLACK_KNIGHT,BLACK_KNIGHT,WHITE_KNIGHT}
            };

            for(int i=0 ;i<ROWS;i++)
            {
                for(int j=0;j<COLUMNS;j++)
                {
                    if(stalemate[i][j]!=tiles[i][j])
                        return false;
                }
            }
            return true;
       }
        int getBottomLeftCorner()
        {
            return tiles[ROWS-1][0];
        }
        int getTopLeftCorner()
        {
            return tiles[0][0];
        }

        int getBottomRightCorner()
        {
            return tiles[ROWS-1][COLUMNS-1];
        }
        int getTopRight()
        {
            return tiles[0][COLUMNS-1];
        }
        int getTopCentre()
        {
            return tiles[0][COLUMNS/2];
        }
        int getBottomCentre()
        {
            return tiles[ROWS-1][COLUMNS/2];
        }
        void makeMove(int start , int end )
        {
            counter++;
            auto startIndices=convertTo2dIndex(start);
            auto endIndices=convertTo2dIndex(end);
            std::swap(tiles[startIndices[0]][startIndices[1]],tiles[endIndices[0]][endIndices[1]]);
            occupied[start-1]=false;occupied[end-1]=true;
            std::cout<<"MOVE : " << counter <<std::endl<<*this<<std::endl<<std::endl;
        }

        void movePath(int start , std::vector<int> edges)
        {
            std::vector<std::vector<int>> path;
            for(int i=0;i<edges.size();i++)
            {
                path.push_back(std::vector<int>{start,edges[i]});
                start=edges[i];
            }

            for(int i=0;i<path.size();i++)
                makeMove(path[i][0],path[i][1]);
        }
        std::vector<int> getPath(int start , int end)//to get the path between 2 tiles
        {   bool finished = false;
            std::vector<bool>occupied=Board::occupied;
            std::vector<int>arr{};
            bool visited[NO_PIECES]={false};
            return getPathDFS(graph[start-1],visited,start,end,arr,occupied,&finished);
        }
//DFS to return the path
        std::vector<int> getPathDFS(std::vector<int> possibleMoves,bool* visited ,int start 
        , int end,std::vector<int>path,std::vector<bool>occupied,bool* finished)
        {   
            std::vector<int> copied;
            if(start==end)
            {
                *finished=true;
                return path;
            }
            visited[start-1]=true;
            for(int i=0;i<possibleMoves.size();i++)
            {
                int element = possibleMoves[i];
                if(visited[element-1] || occupied[element-1])
                    continue;
                occupied[start-1]=false; occupied[element-1]=true;
                path.push_back(element);
                copied=getPathDFS(graph[element-1],visited,element,end,path,occupied,finished);
                if(!(*finished))
                    path.pop_back();
                else
                    return copied;
            }
            return copied;
        }
        void solveGuarini()
        {
            std::cout << *this ; 
            makeMove(1,6);
            makeMove(6,7);
            while(!hasReachedFinal())
            {
                int BOTTOM_CORNERS[]={BOTTOM_LEFT,BOTTOM_RIGHT};
                int TOP_CORNERS[]={TOP_LEFT,TOP_RIGHT};
                int OPTIONS_TOP_LEFT[]={BOTTOM_RIGHT,BOTTOM_CENTRE};
                int OPTIONS_TOP_RIGHT[]={BOTTOM_LEFT,BOTTOM_CENTRE};
                int OPTIONS_BOTTOM_LEFT[]={TOP_RIGHT,TOP_CENTRE};
                int OPTIONS_BOTTOM_RIGHT[]={TOP_LEFT,TOP_CENTRE};
                std::vector<int> path{};
                int start;
                if(hasReachedStaleMate())
                {
                    makeMove(7,6);continue;
                }
                else if(counter==14)
                {
                    makeMove(6,7);makeMove(7,12);
                    continue;
                }
                else if(!isThatTileOccupied(TOP_CENTRE))
               {
                   for(int i=0;i<2;i++)
                   {
                       if(isThatTileWhite(BOTTOM_CORNERS[i]))
                       {
                           path = getPath(BOTTOM_CORNERS[i],TOP_CENTRE);
                           start= BOTTOM_CORNERS[i];
                           if(path.size()>0)
                                break;
                       }
                   }                    
               }
                else if(!isThatTileOccupied(BOTTOM_CENTRE))
                {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileBlack(TOP_CORNERS[i]))
                       {
                           path = getPath(TOP_CORNERS[i],BOTTOM_CENTRE);
                           start = TOP_CORNERS[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
                }
               else if(!isThatTileOccupied(TOP_LEFT))
               {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileWhite(OPTIONS_TOP_LEFT[i]))
                       {
                           path = getPath(OPTIONS_TOP_LEFT[i],TOP_LEFT);
                           start=OPTIONS_TOP_LEFT[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
               }
                               
                else if(!isThatTileOccupied(TOP_RIGHT))
               {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileWhite(OPTIONS_TOP_RIGHT[i]))
                       {
                           path = getPath(OPTIONS_TOP_RIGHT[i],TOP_RIGHT);
                           start=OPTIONS_TOP_RIGHT[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
               }
  
                else if(!isThatTileOccupied(BOTTOM_LEFT))
               {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileBlack(OPTIONS_BOTTOM_LEFT[i]))
                       {
                           path = getPath(OPTIONS_BOTTOM_LEFT[i],BOTTOM_LEFT);
                           start=OPTIONS_BOTTOM_LEFT[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
               }

                else if(!isThatTileOccupied(BOTTOM_RIGHT))
               {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileBlack(OPTIONS_BOTTOM_RIGHT[i]))
                       {
                           path = getPath(OPTIONS_BOTTOM_RIGHT[i],BOTTOM_RIGHT);
                           start=OPTIONS_BOTTOM_RIGHT[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
               }
                movePath(start,path);
            }
        }
};
std::ostream& operator<<(std::ostream& os, const Board& board)
{
    os<<"_______"<<std::endl;
    for(int i=0; i<ROWS;i++)
    {
        for(int j=0;j<COLUMNS;j++)
            os<< '|'<<board.tiles[i][j] << "| ";
        os << std::endl << std::endl;
    }
    os<<"_______"<<std::endl;
    return os;
}
int main()
{
    Board board;
    board.solveGuarini();
}
