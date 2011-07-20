(defparameter *3* (open "/home/martin/0718/pipes/3-to-1" :direction :output
			:if-exists :supersede))

(defparameter *2* (open "/home/martin/0718/pipes/2-to-1" :direction :output
			:if-exists :supersede))

(defparameter *process* nil)

(sb-thread:make-thread 
 #'(lambda () 
     (setf *process* (sb-ext:run-program "/home/martin/0718/pipes/1" '() :wait :nil))
     (sb-ext:process-wait *process*))
 :name "running1")

(dotimes (i 3)
 (format *3* "test~a~%" i)
 (force-output *3*))

(dotimes (i 3)
 (format *2* "test~a~%" i)
 (force-output *2*))


(progn
  (format *2* "t2t~a~%" 1)
  (force-output *2*))

(progn
  (format *3* "t3t~a~%" 1)
  (force-output *3*))