
function x_RoleUseJingWuPoint(MapID, InstanceID, RoleID, Point)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["UseJingWuPoint"], 1)
end


--ע�ắ��
aux.RegisterRoleEvent(26, "x_RoleUseJingWuPoint")