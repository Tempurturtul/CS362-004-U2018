

import junit.framework.TestCase;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!





public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   public void testManualTest()
   {
	   //You can use this function to implement your manual testing
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

	   assertTrue(urlVal.isValid("http://www.google.com"));
//	   assertTrue(urlVal.isValid("https://www.google.com")); // FAILURE: "java.lang.ExceptionInInitializerError: Regular expressions are missing"
	   assertTrue(urlVal.isValid("http://google.com"));
	   assertTrue(urlVal.isValid("http://wikipedia.org"));
	   assertTrue(urlVal.isValid("http://a.co"));
	   assertTrue(urlVal.isValid("http://a.co/b/c")); // BUG: Should be valid.
	   assertTrue(urlVal.isValid("http://a.b.co"));
//	   assertTrue(urlVal.isValid("ftp://foo.com/file.txt")); // FAILURE: "java.lang.ExceptionInInitializerError: Regular expressions are missing"
//	   assertTrue(urlVal.isValid("ftp://ftp.foo.com/sub/sub/sub/file.txt")); // FAILURE: "java.lang.ExceptionInInitializerError: Regular expressions are missing"

	   assertTrue(!urlVal.isValid("google.com"));
	   assertTrue(!urlVal.isValid("google"));
	   assertTrue(!urlVal.isValid(".com"));
   }
   
   
   public void testYourFirstPartition()
   {
	 //You can use this function to implement your First Partition testing	   

   }
   
   public void testYourSecondPartition(){
		 //You can use this function to implement your Second Partition testing	   

   }
   //You need to create more test cases for your Partitions if you need to 
   
   public void testIsValid()
   {
	   //You can use this function for programming based testing

   }
   


}
