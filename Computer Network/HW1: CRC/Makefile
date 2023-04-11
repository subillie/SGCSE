all:
	@g++ crc_encoder_20200422.cc -o crc_encoder
	@gcc decoder.c -o crc_decoder
#	@g++ crc_decoder_20200422.cc -o crc_decoder
	@printf "Done with MAKE!\n"

clean:
	@rm -f crc_encoder crc_decoder
	@rm -f codestream.tx datastream.rx
	@rm -f codedstream.tx codedstream.rx
	@rm -f result.txt
	@printf "Done with MAKE CLEAN!\n"

re: clean all

.Phony: all clean re