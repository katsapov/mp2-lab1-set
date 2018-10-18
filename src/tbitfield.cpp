// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0)
	{
		throw - 1;
	}
	BitLen = len;
	MemLen = ((BitLen + (sizeof(TELEM) * 8 - 1)) / (sizeof(TELEM) * 8));
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
	
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}


TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n > BitLen) throw - 2;
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n > (sizeof(TELEM) * 8) || n < 0) throw - 3;
	return (1 << ((n - 1) % (sizeof(TELEM) * 8)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n > BitLen) throw - 2;
	pMem[GetMemIndex(n)] |= GetMemMask(n % (sizeof(TELEM) * 8));
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n > BitLen) throw - 2;
	pMem[GetMemIndex(n)] &= ~(GetMemMask(n % (sizeof(TELEM) * 8)));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n > BitLen) throw - 2;
	return ((pMem[GetMemIndex(n)] & GetMemMask(n % (sizeof(TELEM) * 8)))>0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)return *this;
	delete[]pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i]) return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (*this == bf)return 0;
	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (BitLen > bf.BitLen)
	{
		TBitField check(BitLen);
		for (int i = 0; i < MemLen; i++)
		{
			check.pMem[i] = pMem[i] | bf.pMem[i];
		}
		return check;
	}
	TBitField check(bf.BitLen);
	for (int i = 0; i < bf.MemLen; i++)
	{
		check.pMem[i] = pMem[i] | bf.pMem[i];
	}
	return check;
}


TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen > bf.BitLen)
	{
		TBitField check(BitLen);
		for (int i = 0; i < MemLen; i++)
		{
			check.pMem[i] = pMem[i] & bf.pMem[i];
		}
		return check;
	}
	TBitField check(bf.BitLen);
	for (int i = 0; i < bf.MemLen; i++)
	{
		check.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return check;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(*this);
	int i = 0;
	for (; i < temp.MemLen - 1; i++)
	{
		temp.pMem[i] = ~pMem[i];
	}
	i *= 8 * sizeof(TELEM);
	for (; i < BitLen; i++)
	{
		if (temp.GetBit(i)) temp.ClrBit(i);
		else temp.SetBit(i);
	}
	return temp;//Оператор отрицания возвращает копию объекта
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int index = 0;
	cin >> index;
	bf.SetBit(index);
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
	{
		if (bf.GetBit(i)) ostr << i;
	}
	return ostr;
}