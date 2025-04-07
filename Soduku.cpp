#include<iostream>
#include<vector>
using namespace std;

bool check(const vector<vector<int>>& grid){ //检查数独格式是否正确
    // 检查行
    for (int i = 0; i < 9; ++i) {
        vector<bool> seen(10, false); // 用于标记数字是否已经出现
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j] != 0) { // 如果不是空单元格
                if (seen[grid[i][j]]) { // 如果数字已经出现过
                    return false; // 数独格式错误
                } else {
                    seen[grid[i][j]] = true; // 标记数字已经出现
                }
            }  
        } 
    }

    // 检查列
    for (int j = 0; j < 9; ++j) {
        vector<bool> seen(10, false); // 用于标记数字是否已经出现
        for (int i = 0; i < 9; ++i) {
            if (grid[i][j]!= 0) { // 如果不是空单元格
                if (seen[grid[i][j]]) { // 如果数字已经出现过
                    return false; // 数独格式错误
                } else {
                    seen[grid[i][j]] = true; // 标记数字已经出现 
                }
            }
        } 
    }

    // 检查3x3子网格
    for (int i=0;i<9;i+=3){
        for(int j=0;j<9;j+=3){
            vector<bool> seen(10, false); // 用于标记数字是否已经出现
            for (int x = i; x < i + 3; ++x) {
                for (int y = j; y < j + 3; ++y) {
                    if (grid[x][y]!= 0) { // 如果不是空单元格
                        if (seen[grid[x][y]]) { // 如果数字已经出现过
                            return false; // 数独格式错误
                        } else {
                            seen[grid[x][y]] = true; // 标记数字已经出现
                        }
                    } 
                }  
            } 
        }
    }
    return true;
}

void my_print(const vector<vector<int>>& grid, const vector<vector<vector<int>>>& possibility){ //输出确定值和候选集
    vector<vector<int>>pending; // 记录不确定格子的坐标位置及其候选值
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j] == 0) { // 如果是不确定的格子
                vector<int> candidates = {i,j}; // 记录候选值
                for (int k = 1; k <= 9; ++k) {
                    if (possibility[i][j][k-1]) { // 如果候选值为真
                        candidates.push_back(k); // 记录候选值
                    }
                }
                cout<<"0 ";
                pending.push_back(candidates); // 记录不确定格子的坐标位置及其候选值
            }
            else{
                cout<<grid[i][j]<<" ";
            }
        }
        cout<<endl;
    }
    if(pending.size()>0){
        cout<<"以下为未确定位置及其候选值："<<endl;
        for(int p=0;p<pending.size();++p){
            cout<<(char)(pending[p][0]+'A')<<pending[p][1]+1<<":";
            for(int q=2;q<pending[p].size();++q){
                cout<<pending[p][q]<<",";
            }
            cout<<endl;
        }
    }

}

bool determine(vector<vector<vector<int>>>& possibility, vector<vector<int>>& grid){
    // 先选定一个未确定的格，再对其每个候选值轮询，若该值在其所在的行和列和宫中的其他格子的候选值中均不出现，则说明此候选值必须填在该格
    // 更新只有一个候选值的格
    bool res=false; // 记录是否有候选值被确定
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
           if(grid[i][j]!=0) continue; // 若已经确定，则跳过

            // 更新只有一个候选值的格
            int count=0;
            int num=0;
            for(int k=0;k<9;++k){ // 统计候选值的个数
                if(possibility[i][j][k]==1){ // 若候选值为真
                    count++; // 候选值个数加1
                    num=k+1; // 记录候选值
                }
            }
            if(count==1){ // 若候选值个数为1
                grid[i][j]=num; // 确定该候选值
                res=true;
                // my_print(grid,possibility); // 输出结果
                continue;
            }

            // 先选定一个未确定的格，再对其每个候选值轮询，若该值在其所在的行或列或宫中的其他格子的候选值中均不出现，则说明此候选值必须填在该格
            for(int k=0;k<9;++k){
               if(possibility[i][j][k]==0) continue; // 该候选值已被排除，跳过
               bool f1=true, f2=true, f3=true; // 用于标记该候选值是否在其所在的行或列或宫中的其他格子的候选值中均不出现
               //检查行
               int m;
               for(m=0;m<9;++m){
                   if(m==j) continue; // 跳过当前格子
                   if(grid[i][m]==0&&possibility[i][m][k]==1||grid[i][m]==k+1) break; // 候选值在同一行的另一位置仍有出现的可能，或在同一轮中被确定
               }
               if(m!=9) f1=false; // 候选值在同一行的另一位置仍有出现的可能
               //检查列
               for(m=0;m<9;++m){
                   if(m==i) continue; // 跳过当前格子
                   if(grid[m][j]==0&&possibility[m][j][k]==1||grid[m][j]==k+1) break; // 候选值在同一列的另一位置仍有出现的可能，或在同一轮中被确定
               }
               if(m!=9) f2=false; // 候选值在同一列的另一位置仍有出现的可能
               //检查3x3子网格
               int row=i/3*3, col=j/3*3; // 计算该候选值所在的3x3子网格的左上角坐标
               int flag=0;
               for(m=row;m<row+3;++m){
                   for(int n=col;n<col+3;++n){
                       if(m==i&&n==j) continue; // 跳过当前格子
                       if(grid[m][n]==0&&possibility[m][n][k]==1||grid[m][n]==k+1) // 候选值在同一宫的另一位置仍有出现的可能，或在同一轮中被确定
                       {
                           flag=1;
                       }
                   }
                   if(flag) break;
               }
               if(flag) f3=false;
               if(f1||f2||f3){ // 候选值在其所在的行或列或宫中的其他格子的候选值中均不出现
                   grid[i][j]=k+1; // 确定该候选值
                   for(int u=0;u<9;++u){ // 排除该候选值在其所在的行和列和宫中的其他格子的候选值
                       if(u==k) continue; // 跳过当前候选值
                       possibility[i][j][u]=0; // 排除其他候选值
                   }
                   res=true;
                //    my_print(grid,possibility); // 输出结果
                   break;
               }
           } 
        } 
    }
    return res;
}

vector<vector<vector<int>>> lastRemainingCellInference(const vector<vector<int>>& ori_grid){
    vector<vector<int>> grid; //拷贝数组
    for(int i=0;i<9;++i){
        vector<int> row;
        for(int j=0;j<9;++j){
            row.push_back(ori_grid[i][j]);
        }
        grid.push_back(row);
    }
    vector<vector<vector<int>>>possibility(9,vector<vector<int>>(9,vector<int>(9,1)));
    if(!check(grid)){ // 数独格式错误
        cout<<"输入数独格式错误"<<endl;
        return possibility;
    }
    // 初始化possibility
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j] != 0) { // 如果已经确定
                for (int k = 0; k < 9; ++k) { // 排除其他候选值
                    if (k!= grid[i][j]-1) { // 如果不是当前确定的值
                        possibility[i][j][k]=0; // 排除其他候选值
                    }
                }
            } 
        } 
    }
    bool update; // 记录是否有候选值被确定
    do{
        update=false;
        // 从有确定值的格子出发，也就是对于现有的所有确定值，更新其所在的行和列和宫中所有未确定格子的候选集。
        for(int i=0;i<9;i++){ // 遍历所有格子
            for(int j=0;j<9;j++){ // 遍历所有格子
                if(grid[i][j]==0) continue; // 若不是确定值，则跳过
                int num=grid[i][j]-1; // 记录确定值
                // 更新行
                for(int m=0;m<9;++m){
                    if(grid[i][m]==0) possibility[i][m][num]=0; // 排除该确定值在同一行的其他格子的候选值
                }
                // 更新列
                for(int m=0;m<9;++m){
                    if(grid[m][j]==0) possibility[m][j][num]=0; // 排除该确定值在同一列的其他格子的候选值
                }
                // 更新宫
                int row=i/3*3, col=j/3*3; // 计算该确定值所在的3x3子网格的左上角坐标
                for(int m=row;m<row+3;++m){ // 遍历该3x3子网格的所有格子
                    for(int n=col;n<col+3;++n){ // 遍历该3x3子网格的所有格子
                        if(grid[m][n]==0) possibility[m][n][num]=0; // 排除该确定值在同一宫的其他格子的候选值
                    }
                }
            }
        }
        // 确定候选值
        update=determine(possibility,grid);
    }while(update); // 如果有新候选值被确定，则有继续更新的可能
    my_print(grid,possibility);
    return possibility;
}

vector<vector<vector<int>>> possibleNumberInference(const vector<vector<int>>& ori_grid){
    vector<vector<int>> grid; //拷贝数组
    for(int i=0;i<9;++i){
        vector<int> row;
        for(int j=0;j<9;++j){
            row.push_back(ori_grid[i][j]);
        }
        grid.push_back(row);
    }
    vector<vector<vector<int>>>possibility(9,vector<vector<int>>(9,vector<int>(9,1)));
    if(!check(grid)){ // 数独格式错误
        cout<<"输入数独格式错误"<<endl;
        return possibility;
    }
    // 初始化possibility
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j] != 0) { // 如果已经确定
                for (int k = 0; k < 9; ++k) { // 排除其他候选值
                    if (k!= grid[i][j]-1) { // 如果不是当前确定的值
                        possibility[i][j][k]=0; // 排除其他候选值
                    }
                }
            } 
        } 
    }
    bool update; // 记录是否有候选值被确定
    do{
        update=false;
        //从还没有确定值的格子出发，根据其所在行，列，宫的确定值，更新其候选集。
        for(int i=0;i<9;i++){ // 遍历所有格子
            for(int j=0;j<9;j++){ // 遍历所有格子
                if(grid[i][j]!=0) continue; // 若已经确定，则跳过
                for(int k=1;k<=9;++k){ // 遍历所有候选值
                    if(possibility[i][j][k-1]==0) continue; // 若该候选值已经被排除，则跳过
                    //检查行
                    for(int m=0;m<9;++m){ // 遍历同一行的所有格子
                        if(grid[i][m]==k) { // 若该候选值在同一行的另一位置仍有出现的可能
                            possibility[i][j][k-1]=0; // 排除该候选值
                            break; // 跳出循环
                        }
                    }
                    if(possibility[i][j][k-1]==0) continue; // 若该候选值已经被排除，则跳过
                    //检查列
                    for(int m=0;m<9;++m){ // 遍历同一列的所有格子
                        if(grid[m][j]==k) { // 若该候选值在同一列的另一位置仍有出现的可能
                            possibility[i][j][k-1]=0; // 排除该候选值
                            break; // 跳出循环
                        }
                    }
                    if(possibility[i][j][k-1]==0) continue; // 若该候选值已经被排除，则跳过
                    //检查3x3子网格
                    int row=i/3*3, col=j/3*3; // 计算该候选值所在的3x3子网格的左上角坐标
                    int flag=0; // 用于标记该候选值是否在其所在的3x3子网格的其他格子的候选值中均不出现
                    for(int m=row;m<row+3;++m){ // 遍历该3x3子网格的所有格子
                        for(int n=col;n<col+3;++n){ // 遍历该3x3子网格的所有格子
                            if(grid[m][n]==k) { // 若该候选值在同一宫的另一位置仍有出现的可能
                                possibility[i][j][k-1]=0; // 排除该候选值
                                flag=1; // 标记该候选值在其所在的3x3子网格的其他格子的候选值中均不出现
                                break; // 跳出循环
                            }
                        }
                        if(flag) break; // 若该候选值在其所在的3x3子网格的其他格子的候选值中均不出现，则跳出循环
                    }
                }
            }
        }
        // 确定候选值
        update=determine(possibility,grid);
    }while(update);
    my_print(grid,possibility);
    return possibility;
}


int main(){

    vector<vector<int>> grid1={
        {2,0,0, 0,7,0, 0,3,8},
        {0,0,0, 0,0,6, 0,7,0},
        {3,0,0, 0,4,0, 6,0,0},
    
        {0,0,8, 0,2,0, 7,0,0},
        {1,0,0, 0,0,0, 0,0,6},
        {0,0,7, 0,3,0, 4,0,0},
    
        {0,0,4, 0,8,0, 0,0,9},
        {0,6,0, 4,0,0, 0,0,0},
        {9,1,0, 0,6,0, 0,0,2}
    };  //测试用例1
    vector<vector<int>> grid2={
        {0,7,0, 0,0,8, 0,2,9},
        {0,0,2, 0,0,0, 0,0,4},
        {8,5,4, 0,2,0, 0,0,0},
    
        {0,0,8, 3,7,4, 2,0,0},
        {0,0,0, 0,0,0, 0,0,0},
        {0,0,3, 2,6,1, 7,0,0},
    
        {0,0,0, 0,9,0, 6,1,2},
        {2,0,0, 0,0,0, 4,0,0},
        {1,3,0, 6,0,0, 0,7,0}
    };  //测试用例2
    vector<vector<int>> grid3={
        {1,0,0, 0,2,0, 3,0,0},
        {0,2,9, 5,0,0, 6,0,0},
        {0,0,0, 0,0,6, 7,0,4},
    
        {2,0,0, 0,3,9, 0,0,0},
        {0,3,0, 1,0,0, 2,9,0},
        {9,6,0, 0,5,2, 0,4,0},
    
        {0,0,0, 8,6,1, 0,3,0},
        {0,9,5, 0,0,0, 0,0,8},
        {6,0,8, 0,0,0, 0,0,2}
    };  //测试用例3

    vector<vector<vector<int>>>possibility1 = lastRemainingCellInference(grid3);
    cout<<"---------------------------------------------------------------------------"<<endl;
    vector<vector<vector<int>>>possibility2 = possibleNumberInference(grid3);

    system("pause"); //输出窗口悬停
    return 0;
}