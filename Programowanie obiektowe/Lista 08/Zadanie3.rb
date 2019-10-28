class Open
    def initialize(message)
        @code = message         # Variable with the same type as 'message'.
    end

    def cipher(key)
        @res = String.new       # Variable with deciphered code.
        @code.each_char do |c|  # For each sign decipher code.
            @res += key[c]
        end
        @code = @res            # Save result to the variable in class.
    end

    def to_str
        @code                   # Return ciphered message (no need to write 'return ...')
    end
end

class Ciphered
    def initialize(message)
        @ciphered = message
    end

    def decipher(key)
        @res = String.new
        key = key.invert        # Swap keys in dict ("a" => "d" becomes "d" => "a" etc.)
        
        @ciphered.each_char do |c|
            @res += key[c]
        end
        
        @ciphered = @res
    end

    def to_str
        @ciphered
    end
end

H = Hash[" " => " ", "a" => "b", "b" => "c", "c" => "d", "d" => "e",
"e" => "f", "f" => "g", "g" => "h", "h" => "i", "i" => "j",
"j" => "k", "k" => "l", "l" => "m", "m" => "n", "n" => "o",
"o" => "p", "p" => "r", "r" => "s", "s" => "t", "t" => "u",
"u" => "v", "v" => "w", "w" => "x", "x" => "y", "y" => "z", "z" => "a"]

# Object we want to cipher.
toCipher = Open.new("abcdefghijklmnoprstuvwxyz")
toCipher.cipher(H)
puts toCipher.to_str    # prints ciphered code.

# Object we want to decipher.
toDecipher = Ciphered.new(toCipher.to_str)
toDecipher.decipher(H)
puts toDecipher.to_str

anotherToCipher = Open.new("there is some text that will become ciphered soon")
anotherToCipher.cipher(H)
puts anotherToCipher.to_str

anotherToDecipher = Ciphered.new(anotherToCipher.to_str)
anotherToDecipher.decipher(H)
puts anotherToDecipher.to_str