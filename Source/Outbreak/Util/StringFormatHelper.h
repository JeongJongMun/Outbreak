#pragma once

class StringFormatHelper
{
public:
	/* Converts an integer to a string with leading zeros. Only POSITIVE integers are supported
	 * @param Number The integer to convert.
	 * @param TotalDigits The total number of digits in the resulting string.
	 * @return A string representation of the number, padded with leading zeros if necessary.
	 */
	static FString ToPaddedString(const int32 Number, const int32 TotalDigits);
	static FString PadLeft(const FString& Source, const int32 TotalLength, const FString& PadChar);
};
