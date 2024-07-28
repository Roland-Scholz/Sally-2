package sallyconv;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;

public class SallyConv {

	public SallyConv(String[] args) {

		try {
			BufferedReader fin = new BufferedReader(new FileReader(args[0]));
			BufferedWriter fout = new BufferedWriter(new FileWriter(args[1]));

			String line;
			String outline;
			String[] tokens;
			String[] jmp;
			String op;
			int address = 0xf000;
			int label;

			ArrayList<Integer> labels = new ArrayList<Integer>();
			ArrayList<String> source = new ArrayList<String>();

			source.add("f000:\t\tORG\t0f000h");

			for (;;) {
				line = fin.readLine();
				if (line == null)
					break;
				tokens = line.split("\\s+");
				for (String token : tokens) {
					System.out.print(token + " ");
				}
				System.out.println();

				// System.out.println(line);
				if (line.startsWith(";") || line.length() < 2) { // comments are passed through
					outline = line;
					source.add(line);
					continue;
				}
				if (tokens.length > 5 && tokens[tokens.length - 1].startsWith("(")) {

					// do lines with (+0ah)

					jmp = tokens[3].split(",");
					if (!tokens[3].contains(",")) {
						label = 0xf000 + Integer.parseInt(tokens[3].substring(1, 4), 16);
						outline = Integer.toHexString(address) + ":\t\t" + tokens[2] + "\t" + Integer.toHexString(label)
								+ "\t\t; " + tokens[tokens.length - 1];
					} else {
						label = 0xf000 + Integer.parseInt(jmp[1].substring(1, 4), 16);
						outline = Integer.toHexString(address) + ":\t\t" + tokens[2] + "\t" + jmp[0] + ","
								+ Integer.toHexString(label) + "\t\t; " + tokens[tokens.length - 1];
					}
					labels.add(label);

				} else {
					outline = Integer.toHexString(address) + ":\t";
					for (int i = 2; i < tokens.length; i++) {
						outline = outline + "\t" + tokens[i];
					}
				}

				if (tokens.length >= 2) {
					address += tokens[1].length() >> 1;
				}
				source.add(outline);

			}

			for (String s : source) {
				if (!s.startsWith(";") && s.length() > 4) {
					address = Integer.parseInt(s.substring(0, 4), 16);
					if (!labels.contains(address)) {
						s = "     " + s.substring(5);
					}
				}
				System.out.println(s);
				fout.write(s + "\n");
			}

//			for (Integer l : labels) {
//				System.out.println(l);
//			}

			fin.close();
			fout.close();

		} catch (

		Exception e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		new SallyConv(args);
	}

}
