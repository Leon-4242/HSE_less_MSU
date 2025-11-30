#pragma once 

#include <iostream> 
#include <cstdlib>
#include <memory>

#include <stdexcept>

//у шаред нет релиз, резет хватит

class CString
{
  //char *str = nullptr;
  std::unique_ptr<char[]> str; //будет nullptr
  //char* str { std::make_shared<char[]>(n) };
  //shared_ptr<char[]> str;
  size_t n=0;

  public: 
//-=-
  void SetZero(); 
  //void Full_clean();  
  void CopyOnly(const CString&b);
  void MoveOnly(CString&b);
//-=-
  CString(size_t k=0, char c=0);//конструктор (обычный)
  CString(const CString &b);//конструктор копирования 
  ~CString();//деструктор
  CString(CString &&b); //новое 
//-=-
  CString& operator=(const CString &b);
  CString& operator=(CString &&b); //новое 
  CString& operator=(char s[]);
  char& operator[](size_t i);
  const char& operator[](size_t i)const;
  CString operator+(const CString &b);
  friend std::ostream& operator<<(std::ostream& out, const CString& b);
//-=-
};
