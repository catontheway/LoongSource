----------------------------------------------------------------------------------------
-------------------------------- �ᱦ���� ------------------------------------------
----------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
-- һЩ����
--------------------------------------------------------------------------------------
BaoXiang_ID						= 4500028						-- ������ƷID

BaoXiang_ID_M04						= 6000214						-- ��ͭ���䱦����ƷID
BaoXiang_ID_M07						= 6000215						-- �������䱦����ƷID
BaoXiang_ID_M107						= 6000216						-- �����䱦����ƷID

MAP_ID_M04					= 3017299663						--��¹m04
MAP_ID_M07					=	3017299407					--����m07
MAP_ID_M107				=	2291720561					--100��������

DuoBaoQiBing_Active_ID	= 155									-- �ᱦ����ID
ERS_HoldChest					= 16777216						-- �ᱦ����Я��ģʽ0x1000000
HoldChest_BuffID				= 9438201						-- �ᱦ����Я��ģʽ0x1000000
ERS_SafeArea						=	16								-- 0x0010,	// ��ȫ��
ERS_Safeguard					= 1									-- ������
ES_Dead								=	0									-- ����

-- ��C++ע��Ľű��¼�
ESRE_OnChaseHolderDie				= 48							-- �ᱦ����Я�������߱�ɱ��ʱ�����¼�
ESRE_ForceLootChase						= 49							-- ����ǿ�Ƶ���
ESIE_CanUse									=	0							-- ��Ʒ�Ƿ����
ESIE_CanDelGroundItem					=	2							-- ���ڵ��ϵ���Ʒ�Ƿ����ɾ��
ESIE_PutDownItem							=	3							-- ������Ʒ
ESIE_CanPickUpItem						=	4							-- �ж��Ƿ���Լ�����Ʒ
ESIE_PickUpItem								=	5							-- ������Ʒʱ����
ESIE_CanPutDown							= 6							-- �ж���Ʒ�Ƿ���Զ���
ESIE_CanDelItemOnRoleInit			= 7							-- ��ɫ��ʼ��ʱ�򣬼�鱳������Ʒ��Ҫɾ����0��ʾ��Ҫɾ���� 1��ʾ����Ҫ��
ESAE_OnTimer									=	1							-- ��ʱ����
ESAE_OnStart									=	2							-- ���ʼ
ESAE_OnEnd									=	3							-- �����
ESAE_OnTimerMin							=	4							-- �ÿ���Ӹ���
ESAE_OnActOpenClose					= 9							-- ����̨������رջ

-- ��ɫ����״̬ö��
EMS_Swim							= 6					-- ��Ӿ
EMS_SwimStand					= 7					-- ��ӾƯ��״̬

-- ������
E_Loot_Swim_Can_Not_Pickup_BaoXiang = 11			-- ��Ӿʱ����ʰȡ����
E_Loot_Role_Level_Limit = 12									-- ��ɫ�ȼ�����20������ʰȡ����
E_Loot_Role_SafeArea_Limit = 13								-- ��ɫ�ڰ�ȫ���ڲ���ʰȡ����
E_Loot_Role_Safeguard_Limit = 17							-- PK����״̬����ʰȡ����

EWeekDay = {["Sunday"] = 1, ["Monday"] = 2, ["Tuesday"] = 3, ["Wednesday"] = 4, ["Thursday"] = 5, ["Friday"] = 6, ["Saturday"] = 7}

Need_Save_To_DB_Count = 14									-- ÿ��������Ҫ���浽���ݿ�����ݸ���

--------------------------------------------------------------------------------------
-- �ű��ֲ�����
--------------------------------------------------------------------------------------
Before_Activity_Broadcast = 0				-- ���ʼǰ���Сʱ����Ĳ��ű�ǣ�0��δ���ţ�1���Ѿ�����
Activity_Data = {}
Activity_Data[BaoXiang_ID_M04 ]  = {Save_To_DB = 1,																				-- �����Ƿ�Ҫ�������ݿ�
															 BaoXiang_SerialID_High = 0,																-- ������Ʒ���кţ���λ��                            
															 BaoXiang_SerialID_Low	= 0,																-- ������Ʒ���кţ���λ��                            
															 BaoXiang_GroupID_High = 0,															-- �������ID����λ��                                  
															 BaoXiang_GroupID_Low = 0,																-- �������ID����λ��                                  
															 BaoXiang_MapID=MAP_ID_M04,														-- �������ڳ���ID                                         
															 BaoXiang_X=0,																					-- ����X����                                                 
															 BaoXiang_Z = 0,																					-- ����Z����                                                 
															 BaoXiang_Holder = 0,																		-- �õ��������ҵĽ�ɫID                           
															 Min_Counter = 0,																				-- ���Ӽ�����                                                
															 Active_Started = 0,																				-- ��Ƿ�ʼ                                             
															 Active_End = 0,																					-- ��Ƿ����                                             
															 BaoXiang_OpenRemainTime = BaoXiang_CDTime,							-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩              
															 Tick_Count = 0,																					-- tick��ʱ                                                    
															 BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval}			-- ����ˢ�±���ĵ���ʱ����λΪ�� ��          

Activity_Data[BaoXiang_ID_M07 ]  = {Save_To_DB = 1,																				-- �����Ƿ�Ҫ�������ݿ�
															 BaoXiang_SerialID_High = 0,																-- ������Ʒ���кţ���λ��                            
															 BaoXiang_SerialID_Low	= 0,																-- ������Ʒ���кţ���λ��                            
															 BaoXiang_GroupID_High = 0,															-- �������ID����λ��                                  
															 BaoXiang_GroupID_Low = 0,																-- �������ID����λ��                                  
															 BaoXiang_MapID=MAP_ID_M07,														-- �������ڳ���ID                                         
															 BaoXiang_X=0,																					-- ����X����                                                 
															 BaoXiang_Z = 0,																					-- ����Z����                                                 
															 BaoXiang_Holder = 0,																		-- �õ��������ҵĽ�ɫID                           
															 Min_Counter = 0,																				-- ���Ӽ�����                                                
															 Active_Started = 0,																				-- ��Ƿ�ʼ                                             
															 Active_End = 0,																					-- ��Ƿ����                                             
															 BaoXiang_OpenRemainTime = BaoXiang_CDTime,							-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩              
															 Tick_Count = 0,																					-- tick��ʱ                                                    
															 BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval}			-- ����ˢ�±���ĵ���ʱ����λΪ�� ��          

Activity_Data[BaoXiang_ID_M107] = {Save_To_DB = 1,																				-- �����Ƿ�Ҫ�������ݿ�
															 BaoXiang_SerialID_High = 0,																-- ������Ʒ���кţ���λ��                            
															 BaoXiang_SerialID_Low	= 0,																-- ������Ʒ���кţ���λ��                            
															 BaoXiang_GroupID_High = 0,															-- �������ID����λ��                                  
															 BaoXiang_GroupID_Low = 0,																-- �������ID����λ��                                  
															 BaoXiang_MapID=MAP_ID_M107,													-- �������ڳ���ID                                         
															 BaoXiang_X=0,																					-- ����X����                                                 
															 BaoXiang_Z = 0,																					-- ����Z����                                                 
															 BaoXiang_Holder = 0,																		-- �õ��������ҵĽ�ɫID                           
															 Min_Counter = 0,																				-- ���Ӽ�����                                                
															 Active_Started = 0,																				-- ��Ƿ�ʼ                                             
															 Active_End = 0,																					-- ��Ƿ����                                             
															 BaoXiang_OpenRemainTime = BaoXiang_CDTime,							-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩              
															 Tick_Count = 0,																					-- tick��ʱ                                                    
															 BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval}			-- ����ˢ�±���ĵ���ʱ����λΪ�� ��          

WeekDay_OpenState = {0, 0, 0, 0, 0, 0, 0}	 -- �ᱦ�������״̬�������տ�ʼ

--------------------------------------------------------------------------------------
-- �ű��ֲ����ߺ���
--------------------------------------------------------------------------------------
-- �������ݵ����ݿ�
function SaveActivityDataToDB(BaoXiang_ID)
--����Ҫ�������ݿ���
--[[	if Activity_Data[BaoXiang_ID].Save_To_DB == 1 then
		local IndexFactor = 0
		if BaoXiang_ID == BaoXiang_ID_M04 then
			IndexFactor = 0
		elseif BaoXiang_ID == BaoXiang_ID_M07 then
			IndexFactor = 1
		elseif BaoXiang_ID == BaoXiang_ID_M107 then
			IndexFactor = 2
		else
			return
		end

		local ScriptDataStartIdx = IndexFactor*Need_Save_To_DB_Count
		act.SetActScriptData(DuoBaoQiBing_Active_ID, Need_Save_To_DB_Count,
						ScriptDataStartIdx			,Activity_Data[BaoXiang_ID].BaoXiang_SerialID_High,															-- ������Ʒ���кţ���λ��                  
						ScriptDataStartIdx+1		,Activity_Data[BaoXiang_ID].BaoXiang_SerialID_Low,															-- ������Ʒ���кţ���λ��                  
						ScriptDataStartIdx+2		,Activity_Data[BaoXiang_ID].BaoXiang_GroupID_High,															-- �������ID����λ��                         
						ScriptDataStartIdx+3		,Activity_Data[BaoXiang_ID].BaoXiang_GroupID_Low,															-- �������ID����λ��                         
						ScriptDataStartIdx+4		,Activity_Data[BaoXiang_ID].BaoXiang_X,																				-- ����X����                                        
						ScriptDataStartIdx+	5		,Activity_Data[BaoXiang_ID].BaoXiang_Z,																				-- ����Z����                                        
						ScriptDataStartIdx+	6		,Activity_Data[BaoXiang_ID].BaoXiang_Holder,																		-- �õ��������ҵĽ�ɫID                  
						ScriptDataStartIdx+	7		,Activity_Data[BaoXiang_ID].Min_Counter,																				-- ���Ӽ�����                                       
						ScriptDataStartIdx+	8		,Activity_Data[BaoXiang_ID].Active_Started,																			-- ��Ƿ�ʼ                                   
						ScriptDataStartIdx+	9		,Activity_Data[BaoXiang_ID].Active_End,																				-- ��Ƿ����                                   
						ScriptDataStartIdx+	10	,Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime,													-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩     
						ScriptDataStartIdx+	11	,Activity_Data[BaoXiang_ID].Tick_Count,																				-- tick��ʱ                                           
						ScriptDataStartIdx+12	 	,Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime)												-- ����ˢ�±���ĵ���ʱ����λΪ�� ��
		act.SaveActScriptData(DuoBaoQiBing_Active_ID)
		Activity_Data[BaoXiang_ID].Save_To_DB = 0	
	end	--]]
end

--�����ݿ��ȡ�����
function LoadActivityData(BaoXiang_ID)
--����Ҫ�������ݿ���
--[[	local IndexFactor = 0
	if BaoXiang_ID == BaoXiang_ID_M04 then
		IndexFactor = 0
	elseif BaoXiang_ID == BaoXiang_ID_M07 then
		IndexFactor = 1
	elseif BaoXiang_ID == BaoXiang_ID_M107 then
		IndexFactor = 2
	else
		return
	end

	local ScriptDataStartIdx = IndexFactor*Need_Save_To_DB_Count	-- ��ʼ����

	Activity_Data[BaoXiang_ID].BaoXiang_SerialID_High,					-- ������Ʒ���кţ���λ��                     
	Activity_Data[BaoXiang_ID].BaoXiang_SerialID_Low,						-- ������Ʒ���кţ���λ��                     
	Activity_Data[BaoXiang_ID].BaoXiang_GroupID_High,					-- �������ID����λ��                           
	Activity_Data[BaoXiang_ID].BaoXiang_GroupID_Low,					-- �������ID����λ��                           
	Activity_Data[BaoXiang_ID].BaoXiang_X,										-- ����X����                                          
	Activity_Data[BaoXiang_ID].BaoXiang_Z,										-- ����Z����                                          
	Activity_Data[BaoXiang_ID].BaoXiang_Holder,								-- �õ��������ҵĽ�ɫID                    
	Activity_Data[BaoXiang_ID].Min_Counter,										-- ���Ӽ�����                                         
	Activity_Data[BaoXiang_ID].Active_Started,									-- ��Ƿ�ʼ                                      
	Activity_Data[BaoXiang_ID].Active_End,											-- ��Ƿ����                                      
	Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime,			-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩       
	Activity_Data[BaoXiang_ID].Tick_Count,											-- tick��ʱ                                             
	Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime =		-- ����ˢ�±���ĵ���ʱ����λΪ�룩   
	act.GetActScriptData(
					DuoBaoQiBing_Active_ID,
					Need_Save_To_DB_Count,
					ScriptDataStartIdx,															-- ������Ʒ���кţ���λ��                  
					ScriptDataStartIdx+1,														-- ������Ʒ���кţ���λ��                  
					ScriptDataStartIdx+2,														-- �������ID����λ��                         
					ScriptDataStartIdx+3,														-- �������ID����λ��                         
					ScriptDataStartIdx+4,														-- ����X����                                        
					ScriptDataStartIdx+5,														-- ����Z����                                        
					ScriptDataStartIdx+6,														-- �õ��������ҵĽ�ɫID                  
					ScriptDataStartIdx+7,														-- ���Ӽ�����                                       
					ScriptDataStartIdx+8,														-- ��Ƿ�ʼ                                   
					ScriptDataStartIdx+9,														-- ��Ƿ����                                   
					ScriptDataStartIdx+10	,													-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩     
					ScriptDataStartIdx+11,													-- tick��ʱ                                           
					ScriptDataStartIdx+12	)													-- ����ˢ�±���ĵ���ʱ����λΪ�룩
	act.SaveActScriptData(DuoBaoQiBing_Active_ID)

	Activity_Data[BaoXiang_ID].Save_To_DB = 0--]]
end

function ResetActiveDataWithBaoXiangID(BaoXiang_ID)
	Activity_Data[BaoXiang_ID].BaoXiang_SerialID_High = 0														-- ������Ʒ���кţ���λ��
	Activity_Data[BaoXiang_ID].BaoXiang_SerialID_Low = 0														-- ������Ʒ���кţ���λ��
	Activity_Data[BaoXiang_ID].BaoXiang_GroupID_High = 0														-- �������ID����λ��
	Activity_Data[BaoXiang_ID].BaoXiang_GroupID_Low = 0														-- �������ID����λ��
	Activity_Data[BaoXiang_ID].BaoXiang_X = 0																			-- ����X����
	Activity_Data[BaoXiang_ID].BaoXiang_Z = 0																			-- ����Z����
	Activity_Data[BaoXiang_ID].BaoXiang_Holder = 0																	-- �õ��������ҵĽ�ɫID
	Activity_Data[BaoXiang_ID].Min_Counter = 0																		-- ���Ӽ�����
	Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime = BaoXiang_CDTime					-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩
	Activity_Data[BaoXiang_ID].Tick_Count = 0																			-- tick��ʱ
	Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval	-- ����ˢ�±���ĵ���ʱ����λΪ�� ��

	Activity_Data[BaoXiang_ID].Save_To_DB = 1		--���ñ������ݿ�
end

-- ���û��ر���
function ResetActiveData()
	ResetActiveDataWithBaoXiangID(BaoXiang_ID_M04)
	ResetActiveDataWithBaoXiangID(BaoXiang_ID_M07)
	ResetActiveDataWithBaoXiangID(BaoXiang_ID_M107)
end

-- �㲥��XX�����XX����XX�����Ѿ�Я���˱�������ץ��ʱ��ȥ����ɣ���
function SendPickUpBaoXiangMsg(BaoXiang_ID)
	if Activity_Data[BaoXiang_ID].BaoXiang_MapID == nil or Activity_Data[BaoXiang_ID].BaoXiang_Holder == nil then
		return
	end
	if Activity_Data[BaoXiang_ID].BaoXiang_MapID == 0 or Activity_Data[BaoXiang_ID].BaoXiang_Holder == nil then
		return
	end
	if Activity_Data[BaoXiang_ID].BaoXiang_X == 0 and Activity_Data[BaoXiang_ID].BaoXiang_Z == 0 then
		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID)
		if Activity_Data[BaoXiang_ID].BaoXiang_X == 0 and Activity_Data[BaoXiang_ID].BaoXiang_Z == 0 then
			return
		end
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100202)
	msg.AddMsgEvent(MsgID, 2, Activity_Data[BaoXiang_ID].BaoXiang_Holder)
	msg.AddMsgEvent(MsgID, 6, Activity_Data[BaoXiang_ID].BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, Activity_Data[BaoXiang_ID].BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, Activity_Data[BaoXiang_ID].BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

--�㲥���ʼ(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
function SendChaseUpdateMsg(BaoXiang_ID)
	if Activity_Data[BaoXiang_ID].BaoXiang_MapID == nil then
		return
	end
	if Activity_Data[BaoXiang_ID].BaoXiang_MapID == 0 then
		return
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100201)
	msg.AddMsgEvent(MsgID, 6, Activity_Data[BaoXiang_ID].BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, Activity_Data[BaoXiang_ID].BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, Activity_Data[BaoXiang_ID].BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

-- ��������߱����ɱ��
function SendChaseHolderBeKilledByRole(KillerID, BaoXiang_ID)
	if Activity_Data[BaoXiang_ID].BaoXiang_MapID == nil then
		return
	end
	
	if Activity_Data[BaoXiang_ID].BaoXiang_MapID == 0 then
		return
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100203)
	msg.AddMsgEvent(MsgID, 2, KillerID)
	msg.AddMsgEvent(MsgID, 2, Activity_Data[BaoXiang_ID].BaoXiang_Holder)
	msg.AddMsgEvent(MsgID, 6, Activity_Data[BaoXiang_ID].BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, Activity_Data[BaoXiang_ID].BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, Activity_Data[BaoXiang_ID].BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

-- ͬ�����俪��ʣ��ʱ��
function SyncOpenChaseRemainTime(BaoXiang_ID)
	if BaoXiang_Holder ~= 0 then
		role.SyncOpenChaseRemainTime(Activity_Data[BaoXiang_ID].BaoXiang_Holder, Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime)
	end
end

-- ���±��䵱ǰ����()
function UpdateBaoXiangPos(RoleID, BaoXiang_ID)
	if Activity_Data[BaoXiang_ID].BaoXiang_MapID == nil then
		return
	end
	if Activity_Data[BaoXiang_ID].BaoXiang_MapID == 0 then
		return
	end

	if RoleID ~= 0 then
		local BaoXiang_Y = 0
		Activity_Data[BaoXiang_ID].BaoXiang_X, Activity_Data[BaoXiang_ID].BaoXiang_Y, Activity_Data[BaoXiang_ID].BaoXiang_Z = unit.GetPosition(Activity_Data[BaoXiang_ID].BaoXiang_MapID, 0, RoleID)
		Activity_Data[BaoXiang_ID].Save_To_DB = 1		--���ñ������ݿ�
	end
end

-- ȥ����ɫ���ϵ�Я��ģʽ״̬
function UnSetHoldChestState(RoleID, BaoXiang_ID)
	role.UnSetRoleState(RoleID, ERS_HoldChest, 1)
	unit.CancelBuff(Activity_Data[BaoXiang_ID].BaoXiang_MapID, 0, RoleID, HoldChest_BuffID)
end

-- ����ɫ���öᱦ����Я��ģʽ״̬
function SetHoldChestState(RoleID, BaoXiang_ID)
	role.SetRoleState(RoleID, ERS_HoldChest, 1)
	unit.AddBuff(Activity_Data[BaoXiang_ID].BaoXiang_MapID, 0, RoleID, HoldChest_BuffID, RoleID)
end

-- �����õ��������ҵĽ�ɫID
function SetBaoXiangHolderRoleID(RoleID, BaoXiang_ID)
	if RoleID == 0 then
		-- ȥ����ɫ���ϵ�Я��ģʽ״̬
		UnSetHoldChestState(Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID)

		-- ������䱻���������ñ��俪��ʣ��ʱ��
		if BaoXiang_Hold_Time_Mod == 0 then
			Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime = BaoXiang_CDTime
		end

		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID)
	else
		-- ����ɫ���öᱦ����Я��ģʽ״̬
		SetHoldChestState(RoleID, BaoXiang_ID)

		-- ����ˢ�±���ĵ���ʱ����λΪ�� ��
		Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval

		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(RoleID, BaoXiang_ID)
	end

	-- �����õ��������ҵĽ�ɫID
	Activity_Data[BaoXiang_ID].BaoXiang_Holder = RoleID

	-- ͬ�����俪��ʣ��ʱ��
	SyncOpenChaseRemainTime(BaoXiang_ID)

	--���ñ������ݿ�
	Activity_Data[BaoXiang_ID].Save_To_DB = 1
end

-- ɾ�������
function RemoveBaoXiang(BaoXiang_ID)
	if Activity_Data[BaoXiang_ID].BaoXiang_Holder ~= 0 then
		role.RemoveFromRole(Activity_Data[BaoXiang_ID].BaoXiang_MapID, 0, Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID, 1, 310)
	else
		map.RemoveGroundItem(Activity_Data[BaoXiang_ID].BaoXiang_MapID, Activity_Data[BaoXiang_ID].BaoXiang_GroupID_High, Activity_Data[BaoXiang_ID].BaoXiang_GroupID_Low)
	end
end

-- ÿ�ջ��ʼ
function ActiveStart(BaoXiang_ID, ActivityStartBroadcast)
	-- ���ǰһ��Ļû�н�����ɾ������
	--if Activity_Data[BaoXiang_ID].Active_End == 0 then
		-- ɾ�������
		RemoveBaoXiang(BaoXiang_ID)
	--end

	-- ȥ����ɫ���ϵ�Я��ģʽ״̬
	UnSetHoldChestState(Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID)

	-- ���û��ر���
	ResetActiveDataWithBaoXiangID(BaoXiang_ID)

	if ActivityStartBroadcast == true then
		-- �㲥�����Я�������߲��ɽ���ʹ��"���͵��ߣ����ߡ��������ִ塢�����Լ���������ˮ"�����ҽ������ϲ�����ʹ���˹涨�еĵ��ߣ����ｫ�Զ������ڽ��в�����ʹ�õ���ǰ�ĵص㣬��������أ�������
		local RuleMsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(RuleMsgID, 102, 100204)
		msg.DispatchWorldMsgEvent(RuleMsgID)
	end

	-- ������ˢ�������ŵ������ͼ��������������Ա�
	-- �㲥���ʼ(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
	ActiveStartAndStartBroadcast(BaoXiang_ID)

	-- �״̬���
	Activity_Data[BaoXiang_ID].Active_Started = 1
	Activity_Data[BaoXiang_ID].Active_End = 0

	--���ñ������ݿ�
	Activity_Data[BaoXiang_ID].Save_To_DB = 1
end

-- �ᱦ��������
function ActiveEnd(BaoXiang_ID, SendGetItemBroadcast, SendEndBroadcast)
	Activity_Data[BaoXiang_ID].Active_End = 1

	if SendGetItemBroadcast == true then
		-- �㲥��&lt;p1&gt;�����Ķᱦ�����Ѿ�������&lt;p2&gt;�Ѿ���&lt;p3&gt;����ʱ�䳬��&lt;p4&gt;���ӡ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100207)
		msg.AddMsgEvent(MsgID, 6, Activity_Data[BaoXiang_ID].BaoXiang_MapID)		--��ͼCRC
		msg.AddMsgEvent(MsgID, 4, BaoXiang_ID)															--����TypeID
		msg.AddMsgEvent(MsgID, 2, Activity_Data[BaoXiang_ID].BaoXiang_Holder)		--roleid
		msg.AddMsgEvent(MsgID, 9, BaoXiang_CDTime/60)											--����ʱ��
		msg.DispatchWorldMsgEvent(MsgID)
	end
	if SendEndBroadcast == true then
		-- ��������㲥
		if Activity_Data[BaoXiang_ID_M04].Active_End == 1 and Activity_Data[BaoXiang_ID_M07].Active_End == 1 and Activity_Data[BaoXiang_ID_M107].Active_End == 1 then
			MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100205)
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end

	-- ȥ����ɫ���ϵ�Я��ģʽ״̬
	UnSetHoldChestState(Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID)
end

function GetRandPosInMap(BaoXiang_ID)
	if BaoXiang_ID ~= BaoXiang_ID_M04 and BaoXiang_ID ~= BaoXiang_ID_M07 then
		return 0,0,0
	end
	RandNum = math.random(BaoXiang_Pos_MaxCnt)
	return BaoXiang_Pos[BaoXiang_ID][RandNum].x, BaoXiang_Pos[BaoXiang_ID][RandNum].y, BaoXiang_Pos[BaoXiang_ID][RandNum].z
end

-- �ᱦ������ʼ��ˢ������������㲥
function ActiveStartAndStartBroadcast(BaoXiang_ID)
	MapID = 0
	X = 0
	Z = 0
	SerialID_High = 0
	SerialID_Low = 0
	GroupID_High = 0
	GroupID_Low = 0
	if BaoXiang_ID == BaoXiang_ID_M04 then
		BaoXiang_X, BaoXiang_Y, BaoXiang_Z= GetRandPosInMap(BaoXiang_ID)
		if BaoXiang_X == 0 and BaoXiang_Y == 0 and BaoXiang_Z == 0 then
			return
		end
		MapID, X, Z, SerialID_High, SerialID_Low, GroupID_High,	GroupID_Low = map.RefreshItemInMap(BaoXiang_ID, Activity_Data[BaoXiang_ID].BaoXiang_MapID, BaoXiang_X, BaoXiang_Y, BaoXiang_Z)
    elseif BaoXiang_ID == BaoXiang_ID_M07 then
		BaoXiang_X, BaoXiang_Y, BaoXiang_Z= GetRandPosInMap(BaoXiang_ID)
		if BaoXiang_X == 0 and BaoXiang_Y == 0 and BaoXiang_Z == 0 then
			return
		end
		MapID, X, Z, SerialID_High, SerialID_Low, GroupID_High,	GroupID_Low = map.RefreshItemInMap(BaoXiang_ID, Activity_Data[BaoXiang_ID].BaoXiang_MapID, BaoXiang_X, BaoXiang_Y, BaoXiang_Z)
    elseif BaoXiang_ID == BaoXiang_ID_M107 then
		MapID, X, Z, SerialID_High, SerialID_Low, GroupID_High,	GroupID_Low = map.PutItemNearRandCreature(BaoXiang_ID, Activity_Data[BaoXiang_ID].BaoXiang_MapID)
	else
		return
	end

	if MapID == nil or X == nil or Z == nil or SerialID_High==nil or SerialID_Low == nil or GroupID_High == nil or GroupID_Low == nil then
		return
	end

	--������ˢ�������ŵ���ͼ��������������Ա�
	Activity_Data[BaoXiang_ID].BaoXiang_X							= X
	Activity_Data[BaoXiang_ID].BaoXiang_Z							= Z
	Activity_Data[BaoXiang_ID].BaoXiang_SerialID_High		= SerialID_High
	Activity_Data[BaoXiang_ID].BaoXiang_SerialID_Low			= SerialID_Low
	Activity_Data[BaoXiang_ID].BaoXiang_GroupID_High		= GroupID_High
	Activity_Data[BaoXiang_ID].BaoXiang_GroupID_Low		= GroupID_Low

	--�㲥ˢ�±���(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
	SendChaseUpdateMsg(BaoXiang_ID)

	--���ñ������ݿ�
	Activity_Data[BaoXiang_ID].Save_To_DB = 1
end

-- �ж϶ᱦ�����Ƿ�ʼ�ˣ����һ�û�н���
function IsActiveEnd(BaoXiang_ID)
	return Activity_Data[BaoXiang_ID].Active_End
end

-- ���±��俪��ʣ��ʱ��
function UpdateOpenRemainTime(BaoXiang_ID)
	if Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime == nil then
		Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime	= BaoXiang_CDTime				-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩
	end

	if IsActiveEnd(BaoXiang_ID) == 1 then
		return
	end

	if Activity_Data[BaoXiang_ID].BaoXiang_Holder ~= 0  then
		Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime = Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime-1

		-- �������ɿ���ʣ��ʱ�䣨��λΪ�룩С��0���ᱦ��������
		if Activity_Data[BaoXiang_ID].BaoXiang_OpenRemainTime < 0 then
			ActiveEnd(BaoXiang_ID, true, true)
		end
	end
end

-- ��������ˢ�±���ʣ��ʱ��
function BaoXiangUpdateRemainTime(BaoXiang_ID)
	if Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime == nil then
		Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval	    -- ����ˢ�±���ĵ���ʱ����λΪ�� ��
	end
	if IsActiveEnd(BaoXiang_ID) == 1 then
		return
	end

	if Activity_Data[BaoXiang_ID].BaoXiang_Holder == 0 then
		Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime = Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime-1

		-- ���ˢ�±���ʣ��ʱ�䣨��λΪ�룩С��0������ˢ�±���
		if Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime < 0 then
			map.RemoveGroundItem(Activity_Data[BaoXiang_ID].BaoXiang_MapID, Activity_Data[BaoXiang_ID].BaoXiang_GroupID_High, Activity_Data[BaoXiang_ID].BaoXiang_GroupID_Low)

			-- �ᱦ������ʼ��ˢ������������㲥
			ActiveStartAndStartBroadcast(BaoXiang_ID)

			-- ����ˢ�±���ĵ���ʱ����λΪ�� ��
			Activity_Data[BaoXiang_ID].BaoXiang_UpdateRemainTime	= BaoXiang_Update_Interval

		end
	end
end

-- ����ǽ����Ƿ��жᱦ����
function IsTodayHaveActive()
	return WeekDay_OpenState[EWeekDay[os.date("%A")]]
end

-- ���ݽ�ɫIDȡ����ID
function GetBaoXiangIDByRoleID(RoleID)
	BaoXiang_ID = 0
	if Activity_Data[BaoXiang_ID_M04].BaoXiang_Holder==RoleID then
		BaoXiang_ID = BaoXiang_ID_M04
	elseif Activity_Data[BaoXiang_ID_M07].BaoXiang_Holder==RoleID then
		BaoXiang_ID = BaoXiang_ID_M07
	elseif Activity_Data[BaoXiang_ID_M107].BaoXiang_Holder==RoleID then
		BaoXiang_ID = BaoXiang_ID_M107
	end
	return BaoXiang_ID
end

function BaoXiang_OnActiveTimer(BaoXiang_ID, ActivityStartBroadcast)
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))

	-- tick��ʱ�����㱦��CDʱ�䣬��λΪ�룩
	Activity_Data[BaoXiang_ID].Tick_Count = Activity_Data[BaoXiang_ID].Tick_Count+1
	if Activity_Data[BaoXiang_ID].Tick_Count == 6 then

		-- ���±��俪��ʣ��ʱ��
		UpdateOpenRemainTime(BaoXiang_ID)

		-- ��������ˢ�±���ʣ��ʱ��
		BaoXiangUpdateRemainTime(BaoXiang_ID)

		Activity_Data[BaoXiang_ID].Tick_Count = 0
	end

	-- ������˽���ʱ���ˣ���û�н�����ǿ�ƽ���
	if Activity_End_Hour == Hour and IsActiveEnd(BaoXiang_ID) == 0 then
		-- ɾ�������
		RemoveBaoXiang(BaoXiang_ID)

		-- �ᱦ��������
		ActiveEnd(BaoXiang_ID, false, true)
	end

	-- ���ʼǰ����Active_Started
	if IsTodayHaveActive() == 1 then
		if Before_Activity_Broadcast == 0 and Hour == One_Hour_Before_Start_Hour and Min == 30 then
			Before_Activity_Broadcast = 1

			-- �㲥���ʼǰ�Ĺ���
			local RuleMsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(RuleMsgID, 102, 100206)
			msg.DispatchWorldMsgEvent(RuleMsgID)
		end

		if Before_Activity_Broadcast == 1 and Hour == One_Hour_Before_Start_Hour and Min == 31 then
			Before_Activity_Broadcast = 0
		end

		if Hour == One_Hour_Before_Start_Hour and Min == 59 and Activity_Data[BaoXiang_ID].Active_Started == 1 then
			Activity_Data[BaoXiang_ID].Active_Started = 0
			
			--���ñ������ݿ�
			Activity_Data[BaoXiang_ID].Save_To_DB = 1
		end

		if Hour == Activity_Start_Hour and Min == 0 and Activity_Data[BaoXiang_ID].Active_Started == 0 then
			ActiveStart(BaoXiang_ID, ActivityStartBroadcast)
		end
	end

	-- �������ݵ����ݿ�
	SaveActivityDataToDB(BaoXiang_ID)
end

function BaoXiang_OnActiveTimeMin(BaoXiang_ID)
	
	if IsActiveEnd(BaoXiang_ID) == 1 then
		return
	end

	-- ������仹û�б���Ҽ���
	if BaoXiang_Holder ~= 0 then
		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID)

		-- ͬ�����俪��ʣ��ʱ��
		SyncOpenChaseRemainTime(BaoXiang_ID)
	end

	Activity_Data[BaoXiang_ID].Min_Counter = Activity_Data[BaoXiang_ID].Min_Counter+1
	if Activity_Data[BaoXiang_ID].Min_Counter > BaoXiang_Pos_Broadcast_Interval then

		-- ������仹û�б���Ҽ���
		if Activity_Data[BaoXiang_ID].BaoXiang_Holder ~= 0 then
			-- �㲥��XX�����XX����XX�����Ѿ�Я���˱�������ץ��ʱ��ȥ����ɣ���
			SendPickUpBaoXiangMsg(BaoXiang_ID)
		else
			-- �㲥(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
			SendChaseUpdateMsg(BaoXiang_ID)
		end

		Activity_Data[BaoXiang_ID].Min_Counter = 0
	end
	
	--���ñ������ݿ�
	Activity_Data[BaoXiang_ID].Save_To_DB = 1
end

function BaoXiang_OnPickUpBaoXiang(BaoXiang_ID, RoleID)
	if IsActiveEnd(BaoXiang_ID) == 1 then
		return
	end

	-- �����õ��������ҵĽ�ɫID
	SetBaoXiangHolderRoleID(RoleID, BaoXiang_ID)

	Activity_Data[BaoXiang_ID].BaoXiang_GroupID_High = 0
	Activity_Data[BaoXiang_ID].BaoXiang_GroupID_Low = 0

	-- �㲥��XX�����XX����XX�����Ѿ�Я���˱�������ץ��ʱ��ȥ����ɣ���
	SendPickUpBaoXiangMsg(BaoXiang_ID)

	--���ñ������ݿ�
	Activity_Data[BaoXiang_ID].Save_To_DB = 1
end

function BaoXiang_CanPickUpBaoXiang(BaoXiang_ID, RoleID)
	-- ���ȼ�
	local RoleLvl = role.GetRoleLevel(Activity_Data[BaoXiang_ID].BaoXiang_MapID, 0, RoleID)
	if RoleLvl == nil then
		return 0		--E_Success
	end
	if Activity_Data[BaoXiang_ID].BaoXiang_ID == ItemID and  (RoleLvl < Active_Min_Level) then
		return E_Loot_Role_Level_Limit		-- �ȼ�������Ҫ��
	end

	-- ����ɫ��Ӿ״̬
	local CurMoveState = role.GetCurMoveState(RoleID)
	if CurMoveState == EMS_Swim	or CurMoveState == EMS_SwimStand then
		return E_Loot_Swim_Can_Not_Pickup_BaoXiang
	end

	-- ����ɫ�ǲ�������ȫ��
	if role.IsRoleInStatus(RoleID, ERS_SafeArea) then
		return E_Loot_Role_SafeArea_Limit
	end

	-- ����ɫ�Ƿ���PK����״̬
	if role.IsRoleInStatus(RoleID, ERS_Safeguard) then
		return E_Loot_Role_Safeguard_Limit
	end

	-- ����Ƿ�������״̬
	if unit.IsInState(Activity_Data[BaoXiang_ID].BaoXiang_MapID, 0, RoleID, ES_Dead) then
		return 999 --����Ҫ��ʾ
	end

	return 0			--E_Success
end

function BaoXiang_OnChaseHolderDie(BaoXiang_ID, RoleID, KillerID)
	if IsActiveEnd(BaoXiang_ID) == 1 then
		return
	end

	if Activity_Data[BaoXiang_ID].BaoXiang_Holder ~= RoleID then
		return
	end

	-- ���±��䵱ǰ����
	UpdateBaoXiangPos(Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID)

	if unit.IsPlayer(KillerID) then
		-- ���㲥��<Msg id="100074" type="1" channel="5" value="���[&lt;p1&gt;]�Ѿ���Я����������[&lt;p2&gt;]ɱ�������������&lt;p3&gt;(&lt;p4&gt;,&lt;p5&gt;)���꣬��ҸϿ�ȥ����ɣ�"/>��
		SendChaseHolderBeKilledByRole(KillerID, BaoXiang_ID)
	else
		--�㲥ˢ�±���(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
		SendChaseUpdateMsg(BaoXiang_ID)
	end

	-- ��������ϵı��䶪��������
	role.PutDownItem(RoleID, BaoXiang_ID)

	-- ���ñ���Я����ID
	SetBaoXiangHolderRoleID(0, BaoXiang_ID)

	-- ���ñ������ݿ�
	Activity_Data[BaoXiang_ID].Save_To_DB = 1
end

function BaoXiang_CanDelBaoXiangOnGround(BaoXiang_ID, SerialID_High, SerialID_Low)
	if IsActiveEnd(BaoXiang_ID) == 1 then
		return 0
	end

	if Activity_Data[BaoXiang_ID].BaoXiang_SerialID_High == SerialID_High and Activity_Data[BaoXiang_ID].BaoXiang_SerialID_Low == SerialID_Low then
		return 1
	else
		return 0
	end
end

function BaoXiang_ForceLootChase(BaoXiang_ID, RoleID)
	if IsActiveEnd(BaoXiang_ID) == 1 then
		return
	end

	if Activity_Data[BaoXiang_ID].BaoXiang_Holder == RoleID then
		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(Activity_Data[BaoXiang_ID].BaoXiang_Holder, BaoXiang_ID)

		-- ��������ϵı��䶪��������
		Activity_Data[BaoXiang_ID].BaoXiang_GroupID_High, Activity_Data[BaoXiang_ID].BaoXiang_GroupID_Low = role.PutDownItem(RoleID, BaoXiang_ID)

		-- ֪ͨ��ұ������
		role.ForceLootChase(Activity_Data[BaoXiang_ID].BaoXiang_Holder)

		-- ���ñ���Я����ID
		SetBaoXiangHolderRoleID(0, BaoXiang_ID)

		--�㲥ˢ�±���(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
		SendChaseUpdateMsg(BaoXiang_ID)

		-- ���ñ������ݿ�
		Activity_Data[BaoXiang_ID].Save_To_DB = 1
	end
end

function BaoXiang_CanPutDownBaoXiang(BaoXiang_ID, SerialID_High, SerialID_Low)
	if IsActiveEnd(BaoXiang_ID) == 1 then
		return 0
	end

	if Activity_Data[BaoXiang_ID].BaoXiang_SerialID_High == SerialID_High and Activity_Data[BaoXiang_ID].BaoXiang_SerialID_Low == SerialID_Low then
		return 1
	else
		return 0
	end
end

--------------------------------------------------------------------------------------
-- �ű���C++��������ע����¼�����
--------------------------------------------------------------------------------------

-- �ᱦ������ʼ
function DuoBaoQiBing_OnActiveStart(ActiveID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	WeekDay_OpenState[1], WeekDay_OpenState[2], WeekDay_OpenState[3], WeekDay_OpenState[4], WeekDay_OpenState[5], WeekDay_OpenState[6], WeekDay_OpenState[7] = act.GetDuoBaoQiBingOpenState()

	-- ���û��ر���
	ResetActiveData()

	-- �������ݿ��д洢�Ļ����
	--LoadActivityData(BaoXiang_ID_M04)
	--LoadActivityData(BaoXiang_ID_M07)
	--LoadActivityData(BaoXiang_ID_M107)

	Activity_Data[BaoXiang_ID_M04].Active_Started				= 0								-- ��Ƿ�ʼ
	Activity_Data[BaoXiang_ID_M04].Active_End					= 1								-- ��Ƿ����
	Activity_Data[BaoXiang_ID_M07].Active_Started				= 0								-- ��Ƿ�ʼ
	Activity_Data[BaoXiang_ID_M07].Active_End					= 1								-- ��Ƿ����
	Activity_Data[BaoXiang_ID_M107].Active_Started				= 0								-- ��Ƿ�ʼ
	Activity_Data[BaoXiang_ID_M107].Active_End					= 1								-- ��Ƿ����
end

-- ÿtick����
function DuoBaoQiBing_OnActiveTimer(ActiveID, Sec)
	if DuoBaoQiBing_trigger == 0 then
		return
	end
	BaoXiang_OnActiveTimer(BaoXiang_ID_M04, true)
	BaoXiang_OnActiveTimer(BaoXiang_ID_M07, false)
	BaoXiang_OnActiveTimer(BaoXiang_ID_M107, false)
end

-- ÿ���Ӹ���
function DuoBaoQiBing_OnActiveTimeMin(ActiveID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	BaoXiang_OnActiveTimeMin(BaoXiang_ID_M04)
	BaoXiang_OnActiveTimeMin(BaoXiang_ID_M07)
	BaoXiang_OnActiveTimeMin(BaoXiang_ID_M107)
end

-- �����
function DuoBaoQiBing_OnActiveEnd(ActiveID)

end

--��Ҽ����䣬���㲥
function DuoBaoQiBing_OnPickUpBaoXiang(ItemID, RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if ItemID == BaoXiang_ID_M04 or ItemID == BaoXiang_ID_M07 or ItemID == BaoXiang_ID_M107 then
		BaoXiang_OnPickUpBaoXiang(ItemID, RoleID)
	end
end

--�ж�����Ƿ��ܼ�����
function DuoBaoQiBing_CanPickUpBaoXiang(ItemID, RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	if ItemID == BaoXiang_ID_M04 or ItemID == BaoXiang_ID_M07 or ItemID == BaoXiang_ID_M107 then
		return BaoXiang_CanPickUpBaoXiang(ItemID, RoleID)
	else
		return -1
	end
end

-- ���б������ұ�ɱ��
function DuoBaoQiBing_OnChaseHolderDie(RoleID, KillerID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	BaoXiang_ID = GetBaoXiangIDByRoleID(RoleID)
	if BaoXiang_ID ~= 0 then
		BaoXiang_OnChaseHolderDie(BaoXiang_ID, RoleID, KillerID)
	end
end

-- �����Ƿ����ɾ��(0��ʾ����ɾ���� 1��ʾ������)
function DuoBaoQiBing_CanDelBaoXiangOnGround(TypeID, SerialID_High, SerialID_Low)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	if TypeID == BaoXiang_ID_M04 or TypeID == BaoXiang_ID_M07 or TypeID == BaoXiang_ID_M107 then
		return BaoXiang_CanDelBaoXiangOnGround(TypeID, SerialID_High, SerialID_Low)
	else
		return -1
	end
end

-- ����ǿ�Ƶ���
function DuoBaoQiBing_ForceLootChase(RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	BaoXiang_ID = GetBaoXiangIDByRoleID(RoleID)
	if BaoXiang_ID ~= 0 then
		BaoXiang_ForceLootChase(BaoXiang_ID, RoleID)
	end
end

-- ���ˣ����䲻�ܶ�����(0��ʾ���ԣ� 1��ʾ������)
function DuoBaoQiBing_CanPutDownBaoXiang(TypeID, SerialID_High, SerialID_Low)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	if TypeID ~= BaoXiang_ID_M04 and TypeID ~= BaoXiang_ID_M07 and TypeID ~= BaoXiang_ID_M107 then
		return 0
	end

	return BaoXiang_CanPutDownBaoXiang(TypeID, SerialID_High, SerialID_Low)
end

-- �����Ƿ��ܿ���
function DuoBaoQiBing_CanOpenBaoXiang(MapID, InstanceID, TypeID, TargetID)
	if DuoBaoQiBing_trigger == 0 then
		return 0, 0
	end

	if TypeID ~= BaoXiang_ID_M04 and TypeID ~= BaoXiang_ID_M07 and TypeID ~= BaoXiang_ID_M107 then
		return 0, 0
	end

	if IsActiveEnd(TypeID) == 1 then
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < Open_Chase_Need_Free_Space[TypeID].Free_Space) then
			--��ʾ��ұ����ռ䲻��
			return 40, false
		end
		return 0, 0
	end

	-- E_UseItem_TreasureInTime		=	61,		// �ᱦ�������δ��������ʱ��
	return 61, 1
end

--��ɫ��ʼ��ʱ�򣬼�鱳������Ʒ��Ҫɾ����0��ʾ��Ҫɾ���� 1��ʾ����Ҫ��
function DuoBaoQiBing_CanDeleteBaoXiang(TypeID, SerialID_High, SerialID_Low)
	if TypeID ~= BaoXiang_ID_M04 and TypeID ~= BaoXiang_ID_M07 and TypeID ~= BaoXiang_ID_M107 then
		return 1
	end

	if Activity_Data[TypeID].BaoXiang_SerialID_High == SerialID_High and Activity_Data[TypeID].BaoXiang_SerialID_Low == SerialID_Low and IsActiveEnd(TypeID) == 1 then
		return 1
	else
		return 0
	end
end

--�����̨����
function DuoBaoQiBing_OnActOpenClose(ActiveID, IsOpen)
	if DuoBaoQiBing_Active_ID ~= ActiveID then
		return
	end
	
	if DuoBaoQiBing_trigger == 0 and IsActiveEnd(BaoXiang_ID) ~= 1 then
		-- ɾ�������
		RemoveBaoXiang(BaoXiang_ID_M04)    
		RemoveBaoXiang(BaoXiang_ID_M07)    
		RemoveBaoXiang(BaoXiang_ID_M107)  

		-- �ᱦ��������
		ActiveEnd(BaoXiang_ID_M04, false, true)
		ActiveEnd(BaoXiang_ID_M07, false, true)
		ActiveEnd(BaoXiang_ID_M107, false, true)
	end

	DuoBaoQiBing_trigger = IsOpen
end

-- ��ע���ʼ������ʱ���²���
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnTimer, "DuoBaoQiBing_OnActiveTimer")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnStart, "DuoBaoQiBing_OnActiveStart")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnEnd, "DuoBaoQiBing_OnActiveEnd")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnTimerMin, "DuoBaoQiBing_OnActiveTimeMin")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnActOpenClose, "DuoBaoQiBing_OnActOpenClose")

aux.RegisterRoleEvent(ESRE_OnChaseHolderDie,	"DuoBaoQiBing_OnChaseHolderDie")
aux.RegisterRoleEvent(ESRE_ForceLootChase,		"DuoBaoQiBing_ForceLootChase")

aux.RegisterItemEvent(BaoXiang_ID_M04, ESIE_CanPickUpItem,					"DuoBaoQiBing_CanPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M04, ESIE_PickUpItem,							"DuoBaoQiBing_OnPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M04, ESIE_CanDelGroundItem,				"DuoBaoQiBing_CanDelBaoXiangOnGround")
--aux.RegisterItemEvent(BaoXiang_ID_M04, ESIE_PutDownItem,					"DuoBaoQiBing_PutDownItem")
aux.RegisterItemEvent(BaoXiang_ID_M04, ESIE_CanPutDown,						"DuoBaoQiBing_CanPutDownBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M04, ESIE_CanUse,								"DuoBaoQiBing_CanOpenBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M04, ESIE_CanDelItemOnRoleInit,		"DuoBaoQiBing_CanDeleteBaoXiang")

aux.RegisterItemEvent(BaoXiang_ID_M07, ESIE_CanPickUpItem,					"DuoBaoQiBing_CanPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M07, ESIE_PickUpItem,							"DuoBaoQiBing_OnPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M07, ESIE_CanDelGroundItem,				"DuoBaoQiBing_CanDelBaoXiangOnGround")
--aux.RegisterItemEvent(BaoXiang_ID_M07, ESIE_PutDownItem,					"DuoBaoQiBing_PutDownItem")
aux.RegisterItemEvent(BaoXiang_ID_M07, ESIE_CanPutDown,						"DuoBaoQiBing_CanPutDownBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M07, ESIE_CanUse,								"DuoBaoQiBing_CanOpenBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M07, ESIE_CanDelItemOnRoleInit,		"DuoBaoQiBing_CanDeleteBaoXiang")

aux.RegisterItemEvent(BaoXiang_ID_M107, ESIE_CanPickUpItem,					"DuoBaoQiBing_CanPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M107, ESIE_PickUpItem,							"DuoBaoQiBing_OnPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M107, ESIE_CanDelGroundItem,				"DuoBaoQiBing_CanDelBaoXiangOnGround")
--aux.RegisterItemEvent(BaoXiang_ID_M107, ESIE_PutDownItem,					"DuoBaoQiBing_PutDownItem")
aux.RegisterItemEvent(BaoXiang_ID_M107, ESIE_CanPutDown,						"DuoBaoQiBing_CanPutDownBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M107, ESIE_CanUse,								"DuoBaoQiBing_CanOpenBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID_M107, ESIE_CanDelItemOnRoleInit,		"DuoBaoQiBing_CanDeleteBaoXiang")

