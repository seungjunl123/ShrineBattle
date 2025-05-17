#pragma once

UENUM()
enum class EDynastyConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EDynastyValidType :uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EDynastySuccessType :uint8
{
	Successful,
	Failed
};

UENUM()
enum class EDynastyCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EDynastyCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class EDynastyGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	VeryHard
};

UENUM(BlueprintType)
enum class EDynastyInputMode : uint8
{
	GameOnly,
	UIOnly
};