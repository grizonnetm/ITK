/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef itkMultiThreader_h
#define itkMultiThreader_h

#include "itkMultiThreaderBase.h"
#include "itkMutexLock.h"
#include "itkThreadSupport.h"
#include "itkIntTypes.h"

namespace itk
{
/** \class MultiThreader
 * \brief A class for performing multithreaded execution
 *
 * Multithreader is a class that provides support for multithreaded
 * execution using Windows or POSIX threads.
 * This class can be used to execute a single
 * method on multiple threads, or to specify a method per thread.
 *
 * \ingroup OSSystemObjects
 *
 * \ingroup ITKCommon
 */

class ITKCommon_EXPORT MultiThreader : public MultiThreaderBase
{
public:
  /** Standard class type aliases. */
  using Self = MultiThreader;
  using Superclass = MultiThreaderBase;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiThreader, MultiThreaderBase);

#if !defined ( ITK_LEGACY_REMOVE )
  /** Set/Get the maximum number of threads to use when multithreading.  It
  * will be clamped to the range [ 1, ITK_MAX_THREADS ] because several arrays
  * are already statically allocated using the ITK_MAX_THREADS number.
  * Therefore the caller of this method should check that the requested number
  * of threads was accepted. Legacy: use MultiThreaderBase to invoke these. */
  static void SetGlobalMaximumNumberOfThreads(ThreadIdType val)
  {
    itkGenericOutputMacro("Warning: SetGlobalMaximumNumberOfThreads \
should now be called on itk::MultiThreaderBase. \
It can affect all MultiThreaderBase's derived classes in ITK");
    Superclass::SetGlobalMaximumNumberOfThreads(val);
  }
  static ThreadIdType GetGlobalMaximumNumberOfThreads()
  {
    itkGenericOutputMacro("Warning: GetGlobalMaximumNumberOfThreads \
should now be called on itk::MultiThreaderBase. \
It can affect all MultiThreaderBase's derived classes in ITK");
    return Superclass::GetGlobalMaximumNumberOfThreads();
  }
#endif

  /** Execute the SingleMethod (as define by SetSingleMethod) using
   * m_NumberOfThreads threads. As a side effect the m_NumberOfThreads will be
   * checked against the current m_GlobalMaximumNumberOfThreads and clamped if
   * necessary. */
  void SingleMethodExecute() override;

  /** Execute the MultipleMethods (as define by calling SetMultipleMethod for
   * each of the required m_NumberOfThreads methods) using m_NumberOfThreads
   * threads. As a side effect the m_NumberOfThreads will be checked against the
   * current m_GlobalMaximumNumberOfThreads and clamped if necessary. */
  void MultipleMethodExecute() override;

  /** Set the SingleMethod to f() and the UserData field of the
   * ThreadInfoStruct that is passed to it will be data.
   * This method (and all the methods passed to SetMultipleMethod)
   * must be of type itkThreadFunctionType and must take a single argument of
   * type void *. */
  void SetSingleMethod(ThreadFunctionType, void *data) override;

  /** Set the MultipleMethod at the given index to f() and the UserData
   * field of the ThreadInfoStruct that is passed to it will be data. */
  void SetMultipleMethod(ThreadIdType index, ThreadFunctionType, void *data) override;

  /** Create a new thread for the given function. Return a thread id
     * which is a number between 0 and ITK_MAX_THREADS - 1. This
   * id should be used to kill the thread at a later time. */
  ThreadIdType SpawnThread(ThreadFunctionType, void *data) override;

  /** Terminate the thread that was created with a SpawnThreadExecute() */
  void TerminateThread(ThreadIdType thread_id) override;

  struct ThreadInfoStruct: MultiThreaderBase::ThreadInfoStruct
    {
    int *ActiveFlag;
    MutexLock::Pointer ActiveFlagLock;
    };

protected:
  MultiThreader();
  ~MultiThreader() ITK_OVERRIDE;
  virtual void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

private:
  ITK_DISALLOW_COPY_AND_ASSIGN(MultiThreader);

  /** An array of thread info containing a thread id
   *  (0, 1, 2, .. ITK_MAX_THREADS-1), the thread count, and a pointer
   *  to void so that user data can be passed to each thread. */
  ThreadInfoStruct m_ThreadInfoArray[ITK_MAX_THREADS];

  /** The methods to invoke. */
  ThreadFunctionType m_SingleMethod;
  ThreadFunctionType m_MultipleMethod[ITK_MAX_THREADS];

  /** Storage of MutexFunctions and ints used to control spawned
   *  threads and the spawned thread ids. */
  int                 m_SpawnedThreadActiveFlag[ITK_MAX_THREADS];
  MutexLock::Pointer  m_SpawnedThreadActiveFlagLock[ITK_MAX_THREADS];
  ThreadProcessIdType m_SpawnedThreadProcessID[ITK_MAX_THREADS];
  ThreadInfoStruct    m_SpawnedThreadInfoArray[ITK_MAX_THREADS];

  /** Internal storage of the data. */
  void *m_SingleData;
  void *m_MultipleData[ITK_MAX_THREADS];

  /** spawn a new thread for the SingleMethod */
  ThreadProcessIdType SpawnDispatchSingleMethodThread(ThreadInfoStruct *);
  /** wait for a thread in the threadpool to finish work */
  void SpawnWaitForSingleMethodThread(ThreadProcessIdType);

  /** Friends of Multithreader.
   * ProcessObject is a friend so that it can call PrintSelf() on its
   * Multithreader. */
  friend class ProcessObject;
};
}  // end namespace itk
#endif
