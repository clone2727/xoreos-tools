/* xoreos-tools - Tools to help with xoreos development
 *
 * xoreos-tools is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos-tools is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos-tools. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  Unicode string handling.
 */

#ifndef COMMON_USTRING_H
#define COMMON_USTRING_H

#include <string>
#include <sstream>
#include <vector>

#include "src/common/types.h"

#include "utf8cpp/utf8.h"

namespace Common {

/** A class holding an UTF-8 string.
 *
 *  WARNING:
 *  Copy constructors and assignment operators copying from std::string and
 *  const char * assume the data is either clean (non-extended) ASCII or
 *  already UTF-8.
 */
class UString {
public:
	typedef utf8::iterator<std::string::const_iterator> iterator;

	// Case sensitive compare
	struct sless : std::binary_function<UString, UString, bool> {
		bool operator() (const UString &str1, const UString &str2) const {
			return str1.less(str2);
		}
	};

	// Case insensitive compare
	struct iless : std::binary_function<UString, UString, bool> {
		bool operator() (const UString &str1, const UString &str2) const {
			return str1.lessIgnoreCase(str2);
		}
	};

	/** Copy constructor. */
	UString(const UString &str);
	/** Construct UString from an UTF-8 string. */
	UString(const std::string &str);
	/** Construct UString from an UTF-8 string. */
	UString(const char *str = "");
	/** Construct UString from the first n bytes of an UTF-8 string. */
	UString(const char *str, int n);
	/** Construct UString by creating n copies of Unicode codepoint c. */
	explicit UString(uint32 c, int n = 1);
	/** Construct UString by copying the characters between [sBegin,sEnd). */
	UString(iterator sBegin, iterator sEnd);
	~UString();

	UString &operator=(const UString &str);
	UString &operator=(const std::string &str);
	UString &operator=(const char *str);

	bool operator==(const UString &str) const;
	bool operator!=(const UString &str) const;
	bool operator<(const UString &str) const;
	bool operator>(const UString &str) const;

	UString operator+(const UString &str) const;
	UString operator+(const std::string &str) const;
	UString operator+(const char *str) const;
	UString operator+(uint32 c) const;

	UString &operator+=(const UString &str);
	UString &operator+=(const std::string &str);
	UString &operator+=(const char *str);
	UString &operator+=(uint32 c);

	int strcmp(const UString &str) const;
	int stricmp(const UString &str) const;

	bool equals(const UString &str) const;
	bool equalsIgnoreCase(const UString &str) const;

	bool less(const UString &str) const;
	bool lessIgnoreCase(const UString &str) const;

	/** Swap the contents of the string with this string's. */
	void swap(UString &str);

	/** Clear the string's contents. */
	void clear();

	/** Return the size of the string, in characters. */
	uint32 size() const;

	/** Is the string empty? */
	bool empty() const;

	/** Return the (utf8 encoded) string data. */
	const char *c_str() const;

	iterator begin() const;
	iterator end() const;

	iterator findFirst(uint32 c) const;
	iterator findLast(uint32 c) const;

	bool beginsWith(const UString &with) const;
	bool endsWith(const UString &with) const;

	bool contains(const UString &what) const;

	void truncate(const iterator &it);
	void truncate(uint32 n);

	void trimLeft();
	void trimRight();
	void trim();

	/** Replace all occurences of a character with another character. */
	void replaceAll(uint32 what, uint32 with);

	/** Convert the string to lowercase. */
	void makeLower();
	/** Convert the string to uppercase. */
	void makeUpper();

	/** Return a lowercased copy of the string. */
	UString toLower() const;
	/** Return an uppercased copy of the string. */
	UString toUpper() const;

	/** Convert an iterator into a numerical position. */
	iterator getPosition(uint32 n)    const;
	/** Convert a numerical position into an iterator. */
	uint32   getPosition(iterator it) const;

	/** Insert character c in front of this position. */
	void insert(iterator pos, uint32 c);
	/** Insert a string in front of this position. */
	void insert(iterator pos, const UString &str);
	/** Replace the character at this position with c. */
	void replace(iterator pos, uint32 c);
	/** Replace the characters at this position with str. */
	void replace(iterator pos, const UString &str);
	/** Erase the character within this range. */
	void erase(iterator from, iterator to);
	/** Erase the character at this position. */
	void erase(iterator pos);

	void split(iterator splitPoint, UString &left, UString &right, bool remove = false) const;

	UString substr(iterator from, iterator to) const;

	/** Formatted printer, works like sprintf(). */
	static UString sprintf(const char *s, ...);

	static uint32 split(const UString &text, uint32 delim, std::vector<UString> &texts);

	static void splitTextTokens(const UString &text, std::vector<UString> &tokens);

	static uint32 toLower(uint32 c);
	static uint32 toUpper(uint32 c);

	static bool isASCII(uint32 c); ///< Is the character an ASCII character?

	static bool isSpace(uint32 c); ///< Is the character an ASCII space character?
	static bool isDigit(uint32 c); ///< Is the character an ASCII digit character?
	static bool isAlpha(uint32 c); ///< Is the character an ASCII alphabetic character?
	static bool isAlNum(uint32 c); ///< Is the character an ASCII alphanumeric character?
	static bool isCntrl(uint32 c); ///< Is the character an ASCII control character?

	static uint32 fromUTF16(uint16 c);

private:
	std::string _string; ///< Internal string holding the actual data.

	uint32 _size;

	void recalculateSize();
};


// Right-binding concatenation operators
static inline UString operator+(const std::string &left, const UString &right) {
	return UString(left) + right;
}

static inline UString operator+(const char *left, const UString &right) {
	return UString(left) + right;
}

} // End of namespace Common

#endif // COMMON_USTRING_H
