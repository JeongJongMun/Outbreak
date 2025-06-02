#include "StringFormatHelper.h"

FString StringFormatHelper::ToPaddedString(const int32 Number, const int32 TotalDigits)
{
	const FString NumberString = FString::FromInt(Number);
	FString PaddedString = PadLeft(NumberString, TotalDigits, TEXT("0"));
	
	return PaddedString;
}

FString StringFormatHelper::PadLeft(const FString& Source, const int32 TotalLength, const FString& PadChar)
{
	if (Source.Len() >= TotalLength)
		return Source;

	const int32 PaddingNeeded = TotalLength - Source.Len();
	FString Padding;
    
	for (int32 i = 0; i < PaddingNeeded; i++)
		Padding += PadChar;
    
	return Padding + Source;
}