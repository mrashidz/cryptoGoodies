/*Following code has been developed for Cryptography Course, the code could be optimized in many ways, but in order to keep the readability performance might have been dissmissed.*/
/*After all this is not a programming course.*/
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <QMap>
#include <math.h>
#include <stdlib.h>
const double alph_char_prob[26] = {8.167,1.492,2.782,4.253,12.702,2.228,2.015,6.094,6.966,0.153,0.772,4.025,2.406,6.749,7.507,1.929,0.095,5.987,6.327,9.056,2.758,0.978,2.360,0.150,1.974,0.074};
const char alph_char[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
static const char *CTXT1 = "YVAPGCPCWHXSCJFDKHFZQITBEVYWXOFSSIPSERLHXSRCCCMMVYCGXHJRRCXSFJRDUSFFLT";
static const char *CTXT2 = "VYCDUJKFSHFOVYCBTHKTYAUFGRIIAFQLEWPCWCBQXRGMCAHDOVLIHTCECPLMYRWIHTCTRDKZCIETIFKDCCNADVPH";
class clsFactorialCalc {
public:
    clsFactorialCalc() {
        this->_sa = false;
        this->_d.clear();
        this->_i = 0;
    }
    void found(quint32 _index) {
        if (this->_sa)
            this->_d.append(_index - this->_i);
        else
            this->_sa = true;
        this->_i = _index;
    }
    inline void refresh() {
        this->_sa = false;
        this->_d.clear();
    }
    QList<quint32> dis() {
        foreach (quint32 d, this->_d) {
            for (quint32 i = 2; i <= d/2; ++i) {
                if (d%i);
                else if (this->_f.keys().contains(i))
                    ++this->_f[i];
                else
                    this->_f.insert(i,1);
            }
        }
        return this->_d;
    }
    QMap<quint32,quint32> calc_possible_key_len() {
        QMap<quint32,quint32> result;

        this->_f.remove(2);
        this->_f.remove(3);
        quint8 c = 0;
        qDebug() <<"Looking Through <KeyLen,Frequency>: "<< this->_f;
        quint32 val = 0,first_highest_val=0,second_highest_val=0,first_key=0,second_key=0;
        foreach (quint32 factorial, this->_f.keys()) {
           val = this->_f.value(factorial);
           if (val == first_highest_val)
               ++c;
           if (val >= first_highest_val) {
               result.remove(second_key);
               second_highest_val = first_highest_val;
               second_key = first_key;

               first_highest_val = val;
               first_key = factorial;
               result.insert(first_key,first_highest_val);
           }
           else if (val > second_highest_val)
               second_highest_val = val;
        }
        if (c>2)
            qDebug() << "!!! There are " << c << " possible key lenght with the same probability - check manually !!!";
        return result;
    }
private:
    bool _sa;
    quint32 _i;
    QList<quint32> _d;
    QMap<quint32,quint32> _f;
};
static clsFactorialCalc f;
void pattern_match_distance_calc(QString _cipher_txt, QString _pattern) {
    f.refresh();
    quint16 No_of_Occurance = 0;    
    int i = 0;
    while (i >= 0) {
            i = _cipher_txt.indexOf(_pattern,i);
            if (i < 0);
            else {
                f.found(i);
                ++i;
                ++No_of_Occurance;
            }
        }
    if(No_of_Occurance >= 2)
        qDebug() << _pattern << " - No. of Occurance: " << No_of_Occurance << " - Distance(s): "<< f.dis() ;
}
QList<quint32> sort_d_list(QList<quint32> _list) {
    QList<quint32> sorted;
    quint16 index;
    quint32 value;
    for (int i = 0; i < _list.size(); ++i)
    {
        value = _list.at(i);
        for (int j = 0; j < _list.size();++j) {
            if ( (i!=j) && (_list.at(j)<value) )
                index = j;
            else
                index = i;
        }
        sorted.insert(i,_list.at(index));
    }
    return sorted;
}
QMap<quint8,quint32> calc_char_rep_cnt(const char *_s, quint32 _size) {
    QMap<quint8,quint32> char_rep_cnt;
    for (quint32 j = 0; j < _size; ++j)
    {
        quint8 id = _s[j] - 0x41;
        if(char_rep_cnt.keys().contains(id))
            ++char_rep_cnt[id];
        else
            char_rep_cnt.insert(id,1);
    }
    return char_rep_cnt;
}
QString shift(QString _txt, quint8 _shift)
{
    quint8 decrypt_shift = (26 - _shift)%26;
    QString shifted;
    quint8 size = _txt.size();
    char *s = new char[size];
    memcpy(s,_txt.toStdString().c_str(),size);
    for(quint8 i = 0; i < size; ++i) {
        quint8 id = s[i] - 0x41;
        quint8 new_id = (id+decrypt_shift)%26;
        shifted.append(alph_char[new_id]);
    }
    delete[] s;
    s= NULL;
    return shifted;
}
qreal calc_chi_square(QMap<quint8,quint32> char_rep_cnt) {
    qreal chi= 0;
    for (int i = 0; i < 26; ++i)
    {
        quint32 C = 0;
        qreal E = alph_char_prob[i]/100;
        if(char_rep_cnt.keys().contains(i))
            C = char_rep_cnt.value(i);
        chi+=(pow((C-E),2)/E);
    }
    return chi;
}
QStringList split(QStringList _cipher_txtz, quint8 _key_size) {
    QStringList cipher_splitted;    
    foreach (QString _cipher_txt, _cipher_txtz)
        for(int i = 0;i < _cipher_txt.size();i += _key_size)
            cipher_splitted.append(_cipher_txt.mid(i,_key_size));
    return cipher_splitted;
}
QStringList splitted_to_ceaser(QStringList _splitted, quint8 _key_size) {
    QStringList ceasers;
    for (int i = 0; i < _key_size; ++i) {
        QString ceaser;
        foreach (QString _c_s, _splitted)
            if (i<_c_s.size())
                ceaser.append(_c_s.mid(i,1));
        ceasers.append(ceaser);
    }
    return ceasers;
}
quint8 guess_ceaser_key(QString _ceaser) {

    qDebug() << "------------------- Calculating Chi ---------------------";

    qDebug() << "Ceaser Cipher"<<_ceaser;
    quint32 size = _ceaser.size();
    qreal lowestChi = 1000000;
    quint8 key = 27;
    for (int i = 0; i < 26; ++i) {
        QString shifted = shift(_ceaser,i+1);
        qDebug() << "Shift: "<<i+1<< shifted;
        QMap<quint8,quint32> tmp = calc_char_rep_cnt(shift(_ceaser,i).toStdString().c_str(),size);
        qDebug() << "Calculating Chi for (cahr,count): "<<tmp;
        qreal chi = calc_chi_square(tmp);
        qDebug() <<"Chi Squared: " << chi;
        if (chi < lowestChi) {
            lowestChi = chi;
            key = i;
        }
    }
    qDebug()<<"Lowest chi: "<< lowestChi << key;

    return key;
}
void decrypt_vigenere(QStringList _cipher_txt_z_splitted, QList<quint8> _k) {
    QString plain_txt,key;
    foreach (quint8 c, _k)
        key.append(alph_char[c]);
    char *s = new char[_k.size()];

    qDebug() << "+++ Decrypting cipher texts with key: "<<key;
    foreach (QString cipher, _cipher_txt_z_splitted) {
        memcpy(s,cipher.toStdString().c_str(),cipher.size());
        for (quint8 i = 0;i<cipher.size();++i) {
            quint8 id = (s[i] - 0x41);
            quint8 decrypt = ((id -_k.at(i))+26)%26;
            plain_txt.append(alph_char[decrypt]);
        }
    }
    qDebug() << plain_txt;
    delete[] s;
}
void perform_kasiski(QStringList _cipher_txt_z) {
    quint32 biggest_cipher_txt_size = 0;
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<< First round >>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "<           Looking For Repatetive Patterns             >";
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    foreach (QString cipher_txt, _cipher_txt_z) {
        qDebug() << "Cipher Text: \"" <<cipher_txt << "\"";
        quint32 size = cipher_txt.size();
        if (size > biggest_cipher_txt_size)  biggest_cipher_txt_size = size;
        for (quint32 i = 0; i < size - 1; ++i)
            for (quint32 j = 2; (j<4)&&(j<size-(i+4)); ++j) {
                QString possible_pattern = cipher_txt.mid(i,j);
                pattern_match_distance_calc(cipher_txt, possible_pattern);
            }
    }
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<< Second round >>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "<                Calculating Possible Key                >";
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    QMap<quint32,quint32> possible_Key_lenght  = f.calc_possible_key_len();
    qDebug() << "________________Most Probable Key Lenghts_________________";
    qDebug() << "(keyLen,Frequency): "<<possible_Key_lenght;



    foreach (quint32 k, possible_Key_lenght.keys()) {
        qDebug() << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        qDebug() << "+          efforts is figuring out the key               +";
        qDebug() << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        qDebug() << "+++ key lenght: "<<k << " #"<<possible_Key_lenght.value(k);
        qDebug() << "+++ Cipher Texts Are\n"<<_cipher_txt_z;
        QStringList splitted = split(_cipher_txt_z,k);
        qDebug() << "+++ Cipher Splitted Based On Key Length\n"<<splitted;
        QStringList ceaserz = splitted_to_ceaser(splitted,k);
        qDebug() << "+++ Ceaser Ciphers Based On Splitted List\n"<<ceaserz;
        QList<quint8> vigenere_key;
        foreach (QString ceaser, ceaserz)        
            vigenere_key.append(guess_ceaser_key(ceaser));
        decrypt_vigenere(splitted,vigenere_key);
    }
}
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    //init with all lowercase or upper case
    QStringList cipher_texts;
    cipher_texts.append(CTXT1);
    cipher_texts.append(CTXT2);
    perform_kasiski(cipher_texts);
    return 0;
}
