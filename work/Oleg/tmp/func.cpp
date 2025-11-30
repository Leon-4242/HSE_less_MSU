#include "fun.hpp"

void CString::SetZero()
{
	str.reset();
	n = 0;
}
//-=-
//тут был фулклин
//-=-
void CString::MoveOnly(CString& b)
{
  n=b.n;
  //str.reset(b.str.get());
  str = std::move(b.str);
  b.SetZero();
}
//-=-
void CString::CopyOnly(const CString& b)
{
	SetZero();
  if(b.n>0)
  {
    str.reset(new char[n=b.n]);
    //str = new char[n=b.n];
    for(size_t i=0;i<n;i++)//может memcpy?
      str.get()[i]=b.str.get()[i];
  }
}
//------------------
CString::CString(CString &&b)
{
  MoveOnly(b);
}
//-=-
CString::~CString()
{}
//-=-
CString::CString(size_t k,char c)
{
  SetZero();
  str.reset(new char[k]);
  //str = new char[k];
  for(size_t i=0; i<k; i++)
    str.get()[i]= c;
  n=k;
}
//-=-
CString::CString(const CString& b)
{
	CopyOnly(b);
}
//---------------------------
CString& CString::operator=(CString &&b)
{
  if(&b!=this)
  {
    SetZero();
    MoveOnly(b);
  }
  return *this;
}

CString& CString::operator=(const CString& b)
{
  if(&b!=this)
  {
    //Full_clean();
    SetZero();
    CopyOnly(b);
  }
  return *this;
}
//-=-
CString& CString::operator=(char s[])
{
  //Full_clean();
  SetZero();

  size_t count = 0;
  while(s[count]!='\0')
    count++;
  str.reset(new char [count]);
  //str = new char[count];
  for(size_t i=0; i<count; i++)
    str.get()[i]=s[i];
  n=count;

  return *this;
}
//-=-
char& CString::operator[](size_t i)
{
  if(i < this->n)
  {
    return str.get()[i];
  }
  throw "Oops";
}
//-=-
const char& CString::operator[](size_t i)const
{
  if(i < this->n)
  {
    return str.get()[i];
  }
  throw "Oops";
}
//-=-
CString CString::operator+(const CString& b)
{
  CString tmp((this->n + b.n));
	size_t i = 0;
	while(this->str.get()[i]!='\0')
  {
    std::cout<<str.get()[i]<<std::endl;
    i++;
  }
	for (size_t i = 0; i <(this->n); i++)
		tmp[i] = this->str.get()[i];
	for (size_t i = this->n; i < (this->n + b.n); i++)
		tmp[i] = b[i - this->n];
	return tmp;
}
//-=-
std::ostream& operator<<(std::ostream& out, const CString& b)
{
	char *word = new char[(b.n)+1];
	for (size_t i = 0; i < b.n; i++)
		word[i] = b[i];    
	word[b.n] = '\0';
	out << "Word is | " << word << " |";
	return out;
  delete[] word;
}
