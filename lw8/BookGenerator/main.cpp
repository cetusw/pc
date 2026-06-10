#include <generator>
#include <iostream>
#include <string>
#include <vector>

struct Book
{
	std::string title;
	std::string author;
	std::vector<std::string> chapters;
};

struct BookChapter
{
	std::string bookTitle;
	std::string bookAuthor;
	std::string chapterTitle;
};

std::ostream& operator<<(std::ostream& os, const BookChapter& chapter)
{
	return os << chapter.bookTitle << " "
			  << chapter.bookAuthor << " "
			  << chapter.chapterTitle;
}

std::generator<BookChapter> ListChapters(const Book& book)
{
	for (const auto& chapter : book.chapters)
	{
		BookChapter bookChapter;
		bookChapter.bookTitle = book.title;
		bookChapter.bookAuthor = book.author;
		bookChapter.chapterTitle = chapter;
		co_yield bookChapter;
	}
}

std::generator<BookChapter> ListBookChapters(const std::vector<Book>& books)
{
	for (const auto& book : books)
	{
		for (const auto& chapter : ListChapters(book))
		{
			co_yield chapter;
		}
	}
}

int main()
{
	const std::vector<Book> books = {
		{ "The Great Gatsby", "F. Scott Fitzgerald", { "Chapter 1", "Chapter 2" } },
		{ "1984", "George Orwell", { "Chapter 1", "Chapter 2", "Chapter 3" } },
		{ "To Kill a Mockingbird", "Harper Lee", { "Chapter 1" } }
	};

	for (const auto& chapter : ListBookChapters(books))
	{
		std::cout << chapter << std::endl;
	}
}
