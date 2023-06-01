class BTDevice {

	btport = null;
	breadinprogress = false;

	async init( callback=null ) {
	  if (this.btport==null) {
		  //Connect to a serial port
		  navigator.serial
			.requestPort()
			.then((port) => {
			    // Connect to `port` or add it to the list of available ports.
				try {
					port.open({ baudRate: 9600});
					this.btport = port;
					if (callback != null) {
						callback(true);
					}
				} catch {
					this.btport = null;
					if (callback != null) {
						callback(false);
					}
				}
			})
			.catch((e) => {
			  // The user didn't select a port.
			  this.btport = null;
			  if (callback != null) {
				callback(false);
			  }
			});
		} else {
			this.btport.close();
			this.btport = null;
			if (callback != null) {
				callback(false);
			}
		}
	}
	
	isReady() {
		return this.btport!=null;
	}
	
	write( str ) {
		if (this.btport!=null) {
			const encoder = new TextEncoder();
			const writer = this.btport.writable.getWriter();
			writer.write(encoder.encode(str));
			writer.releaseLock();
		}
	}
	
	async read( callback ) {
		if (this.btport == null) {
			return false;
		}
		if (this.breadinprogress) {
			return false;
		}
		if (this.btport.readable) {	
			var reader = this.btport.readable.getReader();
			try {
			  const decoder = new TextDecoder();
			  this.breadinprogress = true;
			  const readerData = await reader.read();
			  callback( decoder.decode(readerData.value) );
			  return true;
			} catch (err) {
			  const errorMessage = `error reading data: ${err}`;
			  console.error(errorMessage);
			  return false;
			} finally {
				reader.releaseLock();
				this.breadinprogress = false;
			}
		}
	}
}