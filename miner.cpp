#include "miner.h"

// miner_window::miner(QWidget *parent)
//     : QWidget{parent}
// {}




miner::miner(){
    Row=9;
    Col=9;
    miner_num=10;
    Map=NULL;
}

void miner::Create(){
    int Row=this->Row;
    int Col=this->Col;

    if(Map!=NULL){
        for(int i=0;i<this->Row;i++){
            delete Map[i];
        }
        delete Map;
        Map=NULL;
    }

    Map=new int*[Row];
    for(int i=0;i<Col;i++){
        Map[i]=new int[Col];
    }
    ReSetMiner();
}


void miner::Create(int row,int col,int miner_num){

    //维护传进来的参数
    int new_Row=row;
    int new_Col=col;

    //判空 不为空则删除原有地图
    if(Map!=NULL){
        //这里删除的是上一个地图 那么i理应小于上一个的行数 注意别用错
        for(int i=0;i<this->Row;i++){
            delete Map[i];
        }
        delete Map;
        Map=NULL;
    }
    this->Row=new_Row;
    this->Col=new_Col;
    this->miner_num=miner_num;

    Map=new int*[new_Row];

    for(int i=0;i<new_Col;i++){
        Map[i]=new int[new_Col];
    }
    ReSetMiner();
}

void miner::ReSetMiner(){
    int mine_num=this->miner_num;
    for(int i=0;i<this->Row;i++){
        for(int j=0;j<this->Col;j++){
            Map[i][j]=100;//对Map函数初始化 初始状态为100
        }
    }

    //布雷
    srand(time(NULL));
    for(int i=0;i<mine_num;i++){
        int x=rand()%this->Row;//随机的范围为 0~(Row-1)
        int y=rand()%this->Col;
        if(Map[x][y]!=99){
            Map[x][y]=99;//99 表示此处为雷
        }
        else{
            i--;//让i自减 让下一次循环还是此次
        }
    }

    //生成雷周围3X3格子里的数字

    for(int i=0;i<this->Row;i++){
        for(int j=0;j<this->Col;j++){
            if(Map[i][j]==99){
                for(int x=-1;x<2;x++){
                    for(int y=-1;y<2;y++){
                        //每一个点击行为都需要判断是否越界 小心别敲错i与j
                        if(x+i>=Row||x+i<0||y+j>=Col||y+j<0||Map[i+x][j+y]==99){
                            continue;
                        }

                        Map[x+i][y+j]++;//数字将会是101-108
                    }
                }
            }
        }
    }
    this->winflag=1;
    this->first_time=true;
    remain_time=0;
    remain_miner=mine_num;
}

//判断游戏是否获胜
bool miner::Win(){
    for(int i=0;i<this->Row;i++){
        for(int j=0;j<this->Col;j++){
            //如果仍然有没打开的方块或是有旗子插错了 就未获胜
            if(this->Map[i][j]>99||(this->Map[i][j]>49&&this->Map[i][j]<60)){
                return false;
            }
        }
    }
    this->winflag=2;
    return true;
}


bool miner::Mouse_L_Click(int x,int y){//左键点开
    //先判断游戏是否已经结束
    if(winflag==0||winflag==2){
        return false;
    }

    //再判断这个地方是否越界 或 已被打开
    if(x>=this->Row||y>=this->Col||x<0||y<0||Map[x][y]<=90){
        return false;
    }
    //点开后如果存在数字101-108
    if(Map[x][y]<=108&&Map[x][y]>=101){
        Map[x][y]-=100;
        this->first_time=false;
        Win();
        return true;
    }

    //如果点开后没有存在数字 就向8个方向传播
    if(Map[x][y]==100){
        //没有数字就-100让其为0
        Map[x][y]-=100;
        //左三个
        Mouse_L_Click(x-1,y+1);
        Mouse_L_Click(x-1,y);
        Mouse_L_Click(x-1,y-1);
        //中两个
        Mouse_L_Click(x,y+1);
        Mouse_L_Click(x,y-1);
        //右三个
        Mouse_L_Click(x+1,y+1);
        Mouse_L_Click(x+1,y);
        Mouse_L_Click(x+1,y-1);
    }


    //点开后如果数字为-1 踩雷了
    if(Map[x][y]==99){

        //如果是第一次点击 需要删除当前位置的地雷
        if(this->first_time){

            remain_miner--;
            Map[x][y]=0;
            int boomer=0;

            //获取周围的炸弹数
            for(int i=-1;i<2;i++){
                for(int j=-1;j<2;j++){
                    if(x+i>=0&&x+i<Row&&y+j>=0&&y+j<Col){
                        if(Map[x+i][y+j]==99){
                            boomer++;
                        }
                    }
                }
            }

            //更改周围的数字
            for(int i=-1;i<2;i++){
                for(int j=-1;j<2;j++){
                    //(x||y)的意思是 xy中有一个为1则为1 在下面的判断中是为了防止被点击的Map[x][y]被选中
                    if((x+i<Row)&&(x+i>=0)&&(y+j<Col)&&(y+j>=0)&&(i!=0||j!=0)){
                        if(Map[x+i][y+j]>99){
                            Map[x+i][y+j]--;
                        }
                        // //如果周围有炸弹
                        // if(Map[x+i][y+j]==99){
                        //     boomer++;
                        // }
                    }
                }
            }
            this->first_time=false;
            Map[x][y]=100+boomer;
            Mouse_L_Click(x,y);//再次递归 这次此处不存在地雷
            return true;
        }
        //不是第一次点击 游戏结束 要展示出所有地雷和插错的旗子
        for(int i=0;i<Row;i++){
            for(int j=0;j<Col;j++){
                if(Map[i][j]==99){
                    Map[i][j]=-1;
                }
                //如果ij地方存在旗子 则将其变为-2
                if(Map[i][j]>49&&Map[i][j]<60){
                    Map[i][j]=-2;
                }
            }
        }

        this->winflag=0;

    }
    return true;
}

// 右键插旗子
bool miner::Mouse_R_Click(int x,int y){

    if(this->winflag==0||this->winflag==2){
        return false;
    }

    if(x>Row||x<0||y>Col||y<0||Map[x][y]<49){//
        return false;
    }

    //此处规定Map[x][y]在(40,59)区间内视作旗子
    //这个地方是否已经存在旗子 若存在则删除
    if(Map[x][y]>40&&Map[x][y]<59){
        Map[x][y]+=50;
        remain_miner++;
    }
    //未被打开 就在方格上插旗子
    else if(Map[x][y]>90){
        Map[x][y]-=50;
        remain_miner--;
    }
    return true;

}

bool miner::Mouse_LR_Click(int x,int y){
    if(this->winflag==0||this->winflag==2){
        return false;
    }

    //越界或该处未被打开
    if(x>Row||x<0||y>Col||y<0||Map[x][y]>90){
        return false;
    }
    //如果该处的数字与周围的旗子数一致 就将该数字周围的砖块打开
    if(Map[x][y]==this->around_flag(x,y)){

        Mouse_L_Click(x-1,y+1);
        Mouse_L_Click(x-1,y);
        Mouse_L_Click(x-1,y-1);
        Mouse_L_Click(x,y+1);
        Mouse_L_Click(x,y-1);
        Mouse_L_Click(x+1,y+1);
        Mouse_L_Click(x+1,y);
        Mouse_L_Click(x+1,y-1);
    }
    return true;
}

int miner::around_flag(int x,int y){
    int remain_around_flag=0;
    if(x>=Row||x<0||y>=Col||y<0){
        return -1;
    }

    for(int i=-1;i<2;i++){
        for(int j=-1;j<2;j++){
            //越界了 或是 碰到了不是旗子的砖块 就继续执行
            if(x+i>=Row||x+i<0||y+j>=Col||y+j<0||Map[x+i][y+j]<40||Map[x+i][y+j]>60){
                continue;
            }
            remain_around_flag++;
        }
    }
    return remain_around_flag;
}

