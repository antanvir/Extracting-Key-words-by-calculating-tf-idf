#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<cmath>

using namespace  std ;


string str;
string line[1000],word[100][10000];
string WordListOfAllFiles[1000000];
double tf[100][10000], idf[1000000], tf_idf[1000000], added_tf[1000000];
int num[100][10000], TotalWord[100];
int len=0;
int FileNumber, IndividualWordsInAllFiles;


void Now_MergeForTF_IDF(int left,int middle,int right){

    int i, j, k;
    int n1 = middle - left + 1;
    int n2 =  right - middle;
    string temp;

    double L[n1], R[n2];
    string Lw[n1], Rw[n2];

    for (i = 0; i < n1; i++){
        L[i] = tf_idf[left + i];
        Lw[i] = WordListOfAllFiles[left + i];
    }

    for (j = 0; j < n2; j++){
        R[j] = tf_idf[(middle +1) + j];
        Rw[j] = WordListOfAllFiles[(middle +1) + j];

    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            tf_idf[k] = L[i];
            WordListOfAllFiles[k] = Lw[i];

            i++;
        }
        else
        {
            tf_idf[k] = R[j];
            WordListOfAllFiles[k] = Rw[j];

            j++;
        }
        k++;
    }

    while (i < n1)
    {
        tf_idf[k] = L[i];
        WordListOfAllFiles[k] = Lw[i];

        i++;
        k++;
    }

    while (j < n2)
    {
        tf_idf[k] = R[j];
        WordListOfAllFiles[k] = Rw[j];

        j++;
        k++;
    }
}

void Do_MergeSortForTF_IDF( int left, int right){

    if (left < right)
        {
            int middle = left+(right-left)/2;

            Do_MergeSortForTF_IDF( left, middle);
            Do_MergeSortForTF_IDF( middle+1, right);

            Now_MergeForTF_IDF( left, middle, right);
        }
}
void TF_IDF(){

    for(int k=0; k<IndividualWordsInAllFiles; k++){
        bool found = false;
        double total = 0.0;

        for(int i=0; i<FileNumber; i++){
            for(int j=0; j<TotalWord[i]; j++){

                if(WordListOfAllFiles[k]==word[i][j]){
                    total = total + tf[i][j];
                    found =true;
                    break;
                }
            }
            if(found==true) break;
        }
        added_tf[k] = total;
    }
    for(int k=0; k<IndividualWordsInAllFiles; k++){
        tf_idf[k] = added_tf[k]*idf[k];
    }

}


void IDF(string term, int a){
    int counter=0;
    for(int i=0; i<FileNumber; i++){

        for(int j=0; j<TotalWord[i]; j++){
            if(term==word[i][j]){
                counter++;
                break;
            }
        }

    }
    cout<<"Counter : "<<counter<<endl;
    idf[a] = log10((double)FileNumber/counter);
}

void TF(int TotalWord, int file){

    for(int i=0; i< TotalWord; i++){
        tf[file][i] = (double)num[file][i]/TotalWord;
    }
}

void Now_Merge( int left, int middle, int right, int file)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 =  right - middle;
    string temp;

    int L[n1], R[n2];
    string Lw[n1], Rw[n2];

    for (i = 0; i < n1; i++){
        L[i] = num[file][left + i];
        Lw[i] = word[file][left + i];
    }

    for (j = 0; j < n2; j++){
        R[j] = num[file][(middle +1) + j];
        Rw[j] = word[file][(middle +1) + j];

    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] >= R[j])
        {
            num[file][k] = L[i];
            word[file][k] = Lw[i];

            i++;
        }
        else
        {
            num[file][k] = R[j];
            word[file][k] = Rw[j];

            j++;
        }
        k++;
    }

    while (i < n1)
    {
        num[file][k] = L[i];
        word[file][k] = Lw[i];

        i++;
        k++;
    }

    while (j < n2)
    {
        num[file][k] = R[j];
        word[file][k] = Rw[j];

        j++;
        k++;
    }
}


void Do_MergeSort( int left, int right, int file){

    if (left < right)
    {
        int middle = left+(right-left)/2;

        Do_MergeSort( left, middle, file);
        Do_MergeSort( middle+1, right, file);

        Now_Merge( left, middle, right, file);
    }
}

int StringLength(string str)
{
    int i;
    for ( i = 0; str[i] ; i++);

    return i;

}

string PunctuationCleaningAndLowercasing(string  str,int len)

{
	string habijabi = ",./?'\"':;{}[]\\*()%#@!&+-";
	for(int i=0; i<len; i++)
	{
        if ( str[i] >= 'A' && str[i] <= 'Z' ){
            str[i] = str[i] + 32;
            continue;
        }
		for(int j=0;j<habijabi.length();j++)
		{
			if(str[i]==habijabi[j]) {
                    str[i] = ' ';
                    break;
			}
		}
	}
	return str;
}



int main()
{

    ifstream iFile;

    cout<<"How many text files to be read?\n";
    cin>>FileNumber;

    string FileName[FileNumber];

    cout<<"Input file names :\n";
    for(int i=0; i<FileNumber; i++){
        cin>>FileName[i];
    }
    //FileName = "TextToRead1.txt";

    //iFile.open("TextToRead1.txt");
    for(int file=0; file<FileNumber; file++){

        iFile.open(FileName[file].c_str());

        string str;

        if(iFile.is_open())
        {

            int LineNo=0 ;

            while(getline(iFile,str))
            {

                len = StringLength(str);

                str = PunctuationCleaningAndLowercasing(str,len) ;

                string s ;
                line[LineNo] = str;

                LineNo++;

            }
            iFile.close();

            string pureWordFile="pureWords";
            int t=file;
            t++;
            stringstream strs;
            strs<<t;
            string z = strs.str();

            pureWordFile = pureWordFile+ z+".txt";
            //oFile.open("pureWords.txt");
            cout<<pureWordFile<<endl;
            ofstream oFile;

            oFile.open(pureWordFile.c_str());

            string WordFromTextLine, StopWord;
            TotalWord[file] = 0;

            ifstream iff;
            for(int j =0 ; j < LineNo ; j++){

                stringstream ss(line[j]);

                while(ss>>WordFromTextLine){

                    bool found=false;

                    iff.open("stopWords.txt");

                    if(iff.is_open()){

                        while(iff>>StopWord){

                            if(StopWord==WordFromTextLine){

                                found=true;
                                break;
                            }
                        }
                    }
                    iff.close();

                    if(found==false){

                        oFile<<WordFromTextLine<<" ";
                        word[file][TotalWord[file]]=WordFromTextLine;
                        TotalWord[file]++;
                    }
                }

            }

            for(int i=0; i<TotalWord[file]; i++){

                int WordCount=1;
                if(word[file][i]!="-1"){

                    for(int k=i+1; k<TotalWord[file]; k++){

                        if(word[file][i]==word[file][k]){
                            word[file][k]="-1";
                            WordCount++;
                        }
                    }
                }
                if(word[file][i]!="-1"){
                    num[file][i]=WordCount;

                }
            }
            Do_MergeSort( 0, TotalWord[file]-1, file );
            TF(TotalWord[file], file);

            cout<<"File name = "<<FileName[file]<<"\tTotal word = "<<TotalWord[file]<<endl;
            cout<<"[WORD]\t\t[NUMBER OF PRESENCE]\n";
            cout<<"================================================\n\n";

            for(int i=0; i<TotalWord[file]; i++){
                if(word[file][i]!="-1")
                    cout<<word[file][i]<<"\t\t"<<num[file][i]<<endl;
            }

            cout<<"\n\n\t\t[TF]\n";
            cout<<"================================================\n\n";
            for(int i=0; i<TotalWord[file]; i++){
                if(word[file][i]!="-1")
                    cout<<word[file][i]<<"\t\t"<<tf[file][i]<<endl;
            }
            cout<<endl<<endl;
        }
    }
    int a=0;

    for(int i=0; i<FileNumber; i++){
        for(int j=0; j<TotalWord[i]; j++){
            bool flag = false;
            for(int k=0; k<a; k++){
                if( word[i][j]=="-1" || WordListOfAllFiles[k]==word[i][j]){
                    flag = true;
                    break;
                }
            }
            if(flag==false){
                WordListOfAllFiles[a]=word[i][j];

                IDF(word[i][j],a);
                a++;
            }
        }
    }
    IndividualWordsInAllFiles = a;

    cout<<"\n\n\t\t[IDF]\n";
    cout<<"================================================\n\n";
    for(int i=0; i<a; i++){
        cout<<WordListOfAllFiles[i]<<"\t\t"<<idf[i]<<endl;
    }
    cout<<endl;

    TF_IDF();
    Do_MergeSortForTF_IDF( 0, IndividualWordsInAllFiles-1);

    cout<<"\n\n\t[TF-IDF][Sorted in Increasing Order]\n";
    cout<<"================================================\n\n";
    for(int i=0; i<IndividualWordsInAllFiles; i++){
        cout<<WordListOfAllFiles[i]<<"\t\t"<<tf_idf[i]<<endl;
    }

    cout<<"\n\nTop 5 elements having smallest tf-idf values can be considered as KEY WORDS\n\n"
            <<"So KEY WORDS are :\n"<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    for(int i=0; i<5; i++){
        cout<<WordListOfAllFiles[i]<<endl;
    }
    cout<<"\n\n================================================\n\n";
    return 0;
}

