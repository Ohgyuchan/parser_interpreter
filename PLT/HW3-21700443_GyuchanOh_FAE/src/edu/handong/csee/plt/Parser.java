package edu.handong.csee.plt;

import java.util.ArrayList;

import edu.handong.csee.plt.ast.AST;
import edu.handong.csee.plt.ast.Add;
import edu.handong.csee.plt.ast.App;
import edu.handong.csee.plt.ast.Fun;
import edu.handong.csee.plt.ast.Sub;
import edu.handong.csee.plt.ast.Num;
import edu.handong.csee.plt.ast.Id;

public class Parser {

	AST parse(String exampleCode) {
		ArrayList<String> subExpressions = splitExpressionAsSubExpressions(exampleCode);

		// num
		if(subExpressions.size() == 1 && isNumeric(subExpressions.get(0))) {
			
			return new Num(subExpressions.get(0));
		}
		
		
		// add
		if(subExpressions.get(0).equals("+")) {
			
			return new Add(parse(subExpressions.get(1)),parse(subExpressions.get(2)));
		}
		
		// sub
		if(subExpressions.get(0).equals("-")) {
			
			return new Sub(parse(subExpressions.get(1)),parse(subExpressions.get(2)));
		}
		
		// with
		if(subExpressions.get(0).equals("with")) {
			ArrayList<String> withEx = splitExpressionAsSubExpressions(subExpressions.get(1));
			
			return new App(new Fun(withEx.get(0), parse(subExpressions.get(2))), parse(withEx.get(1)));
		}
		
		// fun
		if(subExpressions.get(0).equals("fun")) {
			ArrayList<String> funEx = splitExpressionAsSubExpressions(subExpressions.get(1));
			
			return new Fun(funEx.get(0), parse(subExpressions.get(2)));
		}
		
		
		// id
		if(subExpressions.size() == 1 && isAlphabetic(subExpressions.get(0))) {
			
			return new Id(subExpressions.get(0));
		}
		
		// app
		if(subExpressions.size() == 2) {
			
			return new App(parse(subExpressions.get(0)), parse(subExpressions.get(1)));
		}

		return null;

	}

	private ArrayList<String> splitExpressionAsSubExpressions(String exampleCode) {

		// deal with brackets first.
		if((exampleCode.startsWith("{") && !exampleCode.endsWith("}"))
				|| (!exampleCode.startsWith("{") && exampleCode.endsWith("}"))) {
			System.out.println("Syntax error");
			System.exit(0);
		}

		if(exampleCode.startsWith("{"))
			exampleCode = exampleCode.substring(1, exampleCode.length()-1);

		return getSubExpressions(exampleCode);
	}

	private ArrayList<String> getSubExpressions(String exampleCode) {

		ArrayList<String> sexpressions = new ArrayList<String>();
		int openingParenthesisCount = 0;
		String strBuffer = "";
		for(int i=0; i < exampleCode.length()  ;i++) {
			if(i==0 || (i==0 && exampleCode.charAt(i) == '{')) {
				strBuffer = strBuffer + exampleCode.charAt(i);
				// if(exampleCode.charAt(i) == '{') {
				// 	openingParenthesisCount++;
				// }
				continue;
			} else if(exampleCode.charAt(i)==' ' && openingParenthesisCount==0){
				// buffer is ready to be a subexpression
				if(!strBuffer.isEmpty()) {
					sexpressions.add(strBuffer);
					strBuffer = ""; // Ready to start a new buffer
				}
				continue;
			} else {
				if(exampleCode.charAt(i)=='{' && openingParenthesisCount==0){
					openingParenthesisCount++;
					// Ready to start a new buffer
					strBuffer = "" + exampleCode.charAt(i);
					continue;
				} else if(exampleCode.charAt(i)=='{') {
					openingParenthesisCount++;
					strBuffer = strBuffer + exampleCode.charAt(i);
					continue;
				} else if(exampleCode.charAt(i)=='}' && openingParenthesisCount>0) {
					openingParenthesisCount--;
					strBuffer = strBuffer + exampleCode.charAt(i);
					continue;
				} else if(exampleCode.charAt(i)=='}') {
					// buffer is ready to be a subexpression
					sexpressions.add(strBuffer);
					continue;
				}
			}
			strBuffer = strBuffer + exampleCode.charAt(i);
		}
		
		sexpressions.add(strBuffer);

		return sexpressions;
	}


	public static boolean isNumeric(String str)
	{
		return str.matches("-?\\d+(\\.\\d+)?");  //match a number with optional '-' and decimal.
	}
	
	public static boolean isAlphabetic(String str)
	{
		return str.matches("^[a-zA-Z]*$");
	}

}
