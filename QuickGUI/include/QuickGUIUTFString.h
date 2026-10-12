#ifndef QUICKGUIUTFSTRING_H
#define QUICKGUIUTFSTRING_H

#include <string>
#include "OgreUTFString.h"
#include "QuickGUIDeprecated.h"

namespace QuickGUI
{
	/**
	* Text as UTF-16 code units, with conversion from UTF-8 and to UTF-8.
	* Each element is one UTF-16 code unit. A code point outside the Basic Multilingual Plane is two elements.
	*/
	class UTFString
	{
	public:
		typedef char16_t code_point;
		typedef unsigned int unicode_char;
		typedef std::u16string::size_type size_type;
		typedef std::u16string::difference_type difference_type;
		typedef std::u16string::iterator iterator;
		typedef std::u16string::const_iterator const_iterator;
		typedef std::u16string::reverse_iterator reverse_iterator;
		typedef std::u16string::const_reverse_iterator const_reverse_iterator;

		static const size_type npos = static_cast<size_type>(-1);

		UTFString()
		{
		}

		UTFString(const char* utf8)
		{
			append(utf8);
		}

		UTFString(const std::string& utf8)
		{
			append(utf8.c_str());
		}

		QUICKGUI_DEPRECATED("Use QuickGUI::UTFString.")
		UTFString(const Ogre::UTFString& s)
		{
			append(s.asUTF8().c_str());
		}

		QUICKGUI_DEPRECATED("Use QuickGUI::UTFString.")
		operator Ogre::UTFString() const
		{
			return Ogre::UTFString(asUTF8());
		}

		UTFString(size_type count, code_point c) :
			mData(count,c)
		{
		}

		/// The text as UTF-8.
		std::string asUTF8() const
		{
			std::string out;
			out.reserve(mData.size());

			for(size_type i = 0; i < mData.size(); ++i)
			{
				unicode_char c = mData[i];
				if((c >= 0xD800) && (c <= 0xDBFF) && ((i + 1) < mData.size()) && (mData[i + 1] >= 0xDC00) && (mData[i + 1] <= 0xDFFF))
				{
					c = 0x10000 + ((c - 0xD800) << 10) + (mData[i + 1] - 0xDC00);
					++i;
				}

				if(c < 0x80)
				{
					out.push_back(static_cast<char>(c));
				}
				else if(c < 0x800)
				{
					out.push_back(static_cast<char>(0xC0 | (c >> 6)));
					out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
				}
				else if(c < 0x10000)
				{
					out.push_back(static_cast<char>(0xE0 | (c >> 12)));
					out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
					out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
				}
				else
				{
					out.push_back(static_cast<char>(0xF0 | (c >> 18)));
					out.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
					out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
					out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
				}
			}

			return out;
		}

		size_type size() const { return mData.size(); }
		size_type length() const { return mData.size(); }
		bool empty() const { return mData.empty(); }
		void clear() { mData.clear(); }
		const code_point* c_str() const { return mData.c_str(); }

		code_point& operator[](size_type index) { return mData[index]; }
		const code_point& operator[](size_type index) const { return mData[index]; }
		code_point& at(size_type index) { return mData.at(index); }
		const code_point& at(size_type index) const { return mData.at(index); }

		iterator begin() { return mData.begin(); }
		iterator end() { return mData.end(); }
		const_iterator begin() const { return mData.begin(); }
		const_iterator end() const { return mData.end(); }
		reverse_iterator rbegin() { return mData.rbegin(); }
		reverse_iterator rend() { return mData.rend(); }
		const_reverse_iterator rbegin() const { return mData.rbegin(); }
		const_reverse_iterator rend() const { return mData.rend(); }

		UTFString substr(size_type index, size_type count = npos) const
		{
			UTFString s;
			s.mData = mData.substr(index,count);
			return s;
		}

		size_type find(const UTFString& s, size_type index = 0) const { return mData.find(s.mData,index); }
		size_type find(const char* utf8, size_type index = 0) const { return mData.find(UTFString(utf8).mData,index); }
		size_type find(code_point c, size_type index = 0) const { return mData.find(c,index); }
		size_type find_first_of(const UTFString& s, size_type index = 0) const { return mData.find_first_of(s.mData,index); }

		void push_back(code_point c) { mData.push_back(c); }

		UTFString& append(const UTFString& s)
		{
			mData.append(s.mData);
			return *this;
		}

		UTFString& append(size_type count, code_point c)
		{
			mData.append(count,c);
			return *this;
		}

		UTFString& append(const char* utf8)
		{
			appendUTF8(utf8);
			return *this;
		}

		UTFString& assign(const UTFString& s)
		{
			mData = s.mData;
			return *this;
		}

		UTFString& insert(size_type index, size_type count, code_point c)
		{
			mData.insert(index,count,c);
			return *this;
		}

		UTFString& insert(size_type index, const UTFString& s)
		{
			mData.insert(index,s.mData);
			return *this;
		}

		iterator insert(iterator where, const code_point& c) { return mData.insert(where,c); }

		UTFString& erase(size_type index = 0, size_type count = npos)
		{
			mData.erase(index,count);
			return *this;
		}

		iterator erase(iterator where) { return mData.erase(where); }
		iterator erase(iterator first, iterator last) { return mData.erase(first,last); }

		UTFString& replace(size_type index, size_type count, const UTFString& s)
		{
			mData.replace(index,count,s.mData);
			return *this;
		}

		UTFString& replace(size_type index, size_type count, const char* utf8)
		{
			mData.replace(index,count,UTFString(utf8).mData);
			return *this;
		}

		UTFString& operator+=(const UTFString& s) { return append(s); }
		UTFString& operator+=(code_point c)
		{
			mData.push_back(c);
			return *this;
		}

		bool operator==(const UTFString& s) const { return mData == s.mData; }
		bool operator!=(const UTFString& s) const { return mData != s.mData; }
		bool operator<(const UTFString& s) const { return mData < s.mData; }

	private:
		void appendUTF8(const char* utf8)
		{
			if(utf8 == 0)
			{
				return;
			}

			const unsigned char* p = reinterpret_cast<const unsigned char*>(utf8);
			while(*p != 0)
			{
				unicode_char c = *p;
				int extra = 0;
				if(c >= 0xF0)
				{
					c &= 0x07;
					extra = 3;
				}
				else if(c >= 0xE0)
				{
					c &= 0x0F;
					extra = 2;
				}
				else if(c >= 0xC0)
				{
					c &= 0x1F;
					extra = 1;
				}
				++p;

				while((extra > 0) && ((*p & 0xC0) == 0x80))
				{
					c = (c << 6) | (*p & 0x3F);
					++p;
					--extra;
				}

				if(c >= 0x10000)
				{
					c -= 0x10000;
					mData.push_back(static_cast<code_point>(0xD800 + (c >> 10)));
					mData.push_back(static_cast<code_point>(0xDC00 + (c & 0x3FF)));
				}
				else
				{
					mData.push_back(static_cast<code_point>(c));
				}
			}
		}

		std::u16string mData;
	};

	inline UTFString operator+(const UTFString& a, const UTFString& b)
	{
		UTFString s(a);
		s.append(b);
		return s;
	}
}

#endif
