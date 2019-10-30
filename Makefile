.PHONY: clean All

All:
	@echo "----------Building project:[ SimpleCommunicator - Debug ]----------"
	@cd "SimpleCommunicator" && "$(MAKE)" -f  "SimpleCommunicator.mk"
clean:
	@echo "----------Cleaning project:[ SimpleCommunicator - Debug ]----------"
	@cd "SimpleCommunicator" && "$(MAKE)" -f  "SimpleCommunicator.mk" clean
