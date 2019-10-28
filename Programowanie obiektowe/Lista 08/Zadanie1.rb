class Fixnum
    def factors
        result = []
        
        1.upto(self) do |x|
            if self % x == 0
                result.push((x))
            end
        end

        return result
    end

    def ackermann(y)
        if self == 0
            return y + 1
        elsif y == 0
            return (self - 1).ackermann(1)
        else
            return (self - 1).ackermann(self.ackermann(y - 1))
        end
    end

    def perfect # are all of the factors of x (except x itself) equal to x?
        fact = self.factors
        if self == fact.inject(:+) - self
            return true
        else 
            return false
        end
    end

    def in_words
        dict = Hash[1 => "one", 2 => "two", 3 => "three", 4 => "four",
        5 => "five", 6 => "six", 7 => "seven", 8 => "eight", 9 => "nine", 0 => "zero"]

        result = String.new
        temp = self

        while temp > 0 do
            result = dict[temp % 10] + " " + result
            temp = temp / 10
        end

        return result
    end
end

$\ = "\n"
print 10.factors
print 1000.factors
print 2.factors

print 2.ackermann(1)

print 6.perfect
print 15.perfect
print 28.perfect

print 105.in_words
